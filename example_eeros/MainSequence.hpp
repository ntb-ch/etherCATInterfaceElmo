#ifndef ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_
#define ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_

#include <eeros/core/Executor.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"
#include <unistd.h>


using namespace etherCATInterface;
using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::logger;


class InitDrives : public Step {
public:
	InitDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		log(log),
		elmoDrives(elmoDrives)
		{ }
		
	int action() {	
		log.info() << "Initializing drives:";
	}
	
	bool checkExitCondition() {
		if (elmoDrives.initAllDrives()) {
			log.info() << "Drives initialized";
			return true;
		}
		return false;
	}
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	double pos;
	Logger& log;
};


class HomingDrives : public Step {
public:
	HomingDrives(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives),
		log(log),
		isHomed(false),
		wait("waiting1", this)
		{
			// 10 second timeout
			this->resetTimeout(); 
			this->setTimeoutTime(10);
			this->setTimeoutBehavior(SequenceProp::abort);
		}
		
	int operator() (int drive) {
		this->drive = drive;
		return start();
	}
		
	int action() {
		
		log.info() << "Homing drive: " << drive;
		elmoDrives.ll_setTargetVelocity(drive, 0);
		elmoDrives.setModeOfOperation(drive, etherCATInterface::cyclicSynchronousVelocity);
        elmoDrives.enableDrive(drive);
        wait(1);
		elmoDrives.ll_setTargetVelocity(drive, 5000);
		
		int attempts = 0;
		while ( attempts < 10 && !elmoDrives.enableCapturingIndexPulse(drive)) {
			wait(0.1);
			attempts++;
		}
	}
	
	bool checkExitCondition() {
		isHomed = elmoDrives.getIndexPulseIsCaptured(drive);
		return isHomed;
	}
	
	bool getIsHomed() { return isHomed; };
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	Logger&  log;
	
	int drive;
	bool isHomed;
	Wait wait;
};



class SetVelocity : public Step {
public:
	SetVelocity(std::string name, Sequencer& sequencer, BaseSequence* caller, SafetySystem& SS, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) : 
		Step(name, this),
		SS(SS),
		elmoDrives(elmoDrives),
		log(log),
		wait("waiting1", this)
		{ }
		
	int operator() (int driveNr, double velocityVal) {
		velocity = velocityVal;
		drive = driveNr;
		return start();
	}

	int action() {
		log.info() << "Set drive '" << drive << "' to velocity: " << velocity;
        
        elmoDrives.ll_setTargetVelocity(drive, 0);
		elmoDrives.setModeOfOperation(drive, etherCATInterface::cyclicSynchronousVelocity);
        elmoDrives.enableDrive(drive);
        wait(1);
		elmoDrives.ll_setTargetVelocity(drive, velocity);
		
        
        
// 		elmoDrives.setModeOfOperation(drive, etherCATInterface::cyclicSynchronousVelocity);
// 		elmoDrives.ll_setTargetVelocity(drive, velocity);
        log.info() << "ll_getStatusWord("<<drive<<"): 0x" << elmoDrives.getDriveStatusStringElmo(drive);
	}
	
private:
	SafetySystem& SS;
	EtherCATInterfaceElmo& elmoDrives;
	Logger&  log;
	
	int drive;
	double velocity;
	Wait wait;
};


// ////////////////////////////////////////////////////////////////////////////
// MainSequence
// ////////////////////////////////////////////////////////////////////////////

class MainSequence : public Sequence {
public:
	MainSequence(std::string name, Sequencer& sequencer, SafetySystem& SS, MySafetyProperties& safetyProp, MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, Logger& log) :  
					Sequence(name, sequencer),
					sequencer(sequencer),
					SS(SS),
					safetyProp(safetyProp),
					CS(CS),
// 					log('S'),
					log(log),
					elmoDrives(elmoDrives),
					wait("waiting1", this),
					step_initDrives("initDrives", sequencer, this, SS, CS, elmoDrives, log),
					step_homingDrives("homingDrives", sequencer, this, SS, CS, elmoDrives, log),
					step_setVelocity("homingDrives", sequencer, this, SS, CS, elmoDrives, log)
					{
// 		StreamLogWriter w(std::cout);
// 		Logger log('P');
// 		log.set(w);
// 		w.show(LogLevel::INFO);
		log.info() << "Sequence created: " << name;
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
	}
	
	
	
	int action() {
		log.info() << "ACTION ";
		// Initialize signal checkers
		CS.velocityChecker.setActiveLevel(safetyProp.slDrivesDisabled);
		CS.velocityChecker.registerSafetyEvent(SS, safetyProp.doEmergency);
		CS.velocityChecker.setLimits(-100000, 100000);
// 		wait(2);
		CS.velocityChecker.reset();
        
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
        elmoDrives.ll_setControlWord(0, cwc_faultReset);
        elmoDrives.ll_setControlWord(1, cwc_faultReset);
        wait(1);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
        elmoDrives.ll_setControlWord(0, cwc_shutdown);
        elmoDrives.ll_setControlWord(1, cwc_shutdown);
        wait(1);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
// 		step_initDrives();
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);   

        
//         // homing drives with index pulse (manual switch)
		SS.triggerEvent(safetyProp.enableDrives);
		step_homingDrives(1);
		SS.triggerEvent(safetyProp.disableDrives);
		if ( step_homingDrives.getIsHomed() )  {
			elmoDrives.setOffsetAtIndexPos(1, 0);
			log.info() << "Drives homed";
			log.info() << "Index pulse captured at:      " << elmoDrives.getCapturedPosition(1);
			log.info() << "getPosition(1):               " << elmoDrives.getPosition(1);
			log.info() << "ll_getPositionActualValue(1): " << elmoDrives.ll_getPositionActualValue(1);
		}
		else {
			log.info() << "Drives NOT homed due to timeout";
		}
		log.info();
		
		
// 		simple set velocity test
        /////////////////////////
		log.info() << "Set velocity test started";
		log.info() << "/////////////////////////";
		SS.triggerEvent(safetyProp.enableDrives);
		step_setVelocity(0, 0);
		elmoDrives.enableDrive(0);
		step_setVelocity(1, 0);
		elmoDrives.enableDrive(0);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
		wait(1);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
		step_setVelocity(0, -5000);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
		wait(4);
			log.info() << "SS level:                     " << SS.getCurrentLevel();
			log.info() << "DI:                           0x" << std::hex << CS.getElmos.getOutDigitalInputs().getSignal().getValue();
			log.info() << "Velocity:                     " << CS.getElmos.getOutVelocity().getSignal().getValue();
			log.info() << "Vel SC:                       " << CS.demuxVelocities.getOut(1).getSignal().getValue();
			log.info() << "getPosition(1):               " << elmoDrives.getPosition(1);
			log.info() << "ll_getPositionActualValue(1): " << elmoDrives.ll_getPositionActualValue(1);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
			log.info();
            elmoDrives.enableDrive(0);
            elmoDrives.enableDrive(1);
		wait(4);
			log.info() << "SS level:                     " << SS.getCurrentLevel();
			log.info() << "DI:                           0x" << std::hex << CS.getElmos.getOutDigitalInputs().getSignal().getValue();
			log.info() << "Velocity:                     " << CS.getElmos.getOutVelocity().getSignal().getValue();
			log.info() << "Vel SC:                       " << CS.demuxVelocities.getOut(1).getSignal().getValue();
			log.info() << "getPosition(1):               " << elmoDrives.getPosition(1);
			log.info() << "ll_getPositionActualValue(1): " << elmoDrives.ll_getPositionActualValue(1);
			log.info() << "ll_getStatusWord(0): 0x" << elmoDrives.getDriveStatusStringElmo(0);
			log.info() << "ll_getStatusWord(1): 0x" << elmoDrives.getDriveStatusStringElmo(1);
			log.info();
		wait(4);
		step_setVelocity(1, 0);
		SS.triggerEvent(safetyProp.disableDrives);
		
		
		// timingPerformanceTest/*
// 		log.info() << "timingPerformanceTest started";*/
// 		elmoDrives.ll_setTargetTorque(0, 0);
// 		elmoDrives.setModeOfOperation(0, etherCATInterface::cyclicSynchronousTorque);
// 		SS.triggerEvent(safetyProp.enableDrives);
// 		CS.setElmos.setTargetTorqueByCS = true;
// 		CS.timingPerformanceTester.enable();
// 		wait(1);
// 		log.info() << "timingPerformanceTest without waiting";
// 		elmoDrives.getEtherCATStack()->getInstance()->stopWaitingForEeros();
// 		wait(1);
// 		CS.timingPerformanceTester.disable();
// 		CS.setElmos.setTargetTorqueByCS = false;
// 		elmoDrives.disableAllDrives();
		
		
		for(int i=0; i<30 and sequencer.running; i++) {
			log.info() << "SS level:                     " << SS.getCurrentLevel();
			log.info() << "DI:                           0x" << std::hex << CS.getElmos.getOutDigitalInputs().getSignal().getValue();
			log.info() << "Velocity:                     " << CS.getElmos.getOutVelocity().getSignal().getValue();
			log.info() << "Vel SC:                       " << CS.demuxVelocities.getOut(1).getSignal().getValue();
			log.info() << "getPosition(1):               " << elmoDrives.getPosition(1);
			log.info() << "ll_getPositionActualValue(1): " << elmoDrives.ll_getPositionActualValue(1);
			log.info() << "ll_getStatusWord(0): " << elmoDrives.ll_getStatusWord(0);
			log.info() << "ll_getStatusWord(1): " << elmoDrives.ll_getStatusWord(1);
			log.info();
			
			if ( i%2 == 0 )
				CS.constantDigitalOut.setValue(0xFFFFFFFF);
// 				CS.setElmos.getInDigitalOutput().getSignal().setValue<uint32_t>(0xFFFFFFFF);
// 				elmoDrives.ll_setDigitalOutput(0, 0xFFFFFFFF);
			else
				CS.constantDigitalOut.setValue(0x0);
// 				CS.setElmos.getInDigitalOutput().getSignal().setValue<uint32_t>(0x0);
// 				elmoDrives.ll_setDigitalOutput(0, 0x0);
				
			wait(1);
		}
		
		
// 		wait(30);
		log.info() << "MainSequence finished";
		
		SS.triggerEvent(safetyProp.switchOff);
	}
private:
	Wait wait;
	InitDrives step_initDrives;
	HomingDrives step_homingDrives;
	SetVelocity step_setVelocity;
	
	double angle;
	
	Sequencer& sequencer;
	SafetySystem& SS;
	MyControlSystem& CS;
	Logger& log;
	MySafetyProperties& safetyProp;
	EtherCATInterfaceElmo& elmoDrives;
};

#endif // ETHERCAT_EXAMPLES_SEQUENCER_MAINSEQUENCE_HPP_ 
