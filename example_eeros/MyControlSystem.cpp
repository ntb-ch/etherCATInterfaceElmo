#include "MyControlSystem.hpp"
#include <eeros/core/Executor.hpp>
#include <float.h>

#include <eeros/task/Periodic.hpp>
#include <eeros/core/PeriodicCounter.hpp>

using namespace eeros::control;

MyControlSystem::MyControlSystem(double ts, EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal, Logger& log) :
// log('C'),
log(log),
elmoDrives(elmoDrives),
numberOfDrivesTotal(numberOfDrivesTotal),
getElmos(elmoDrives, numberOfDrivesTotal),
demuxEncoders(),
demuxVelocities(),
demuxTorque(),
velocityChecker(-DBL_MAX, DBL_MAX),
constantDigitalOut(0),
constantDigitalOut1(0),
constantTargetTorque0(0),

timingPerformanceTester(),

muxDigitalOut(),
muxTorqueOut(),
printNumber(log, "Encoder: ", "", 200, false),
setElmos(elmoDrives, numberOfDrivesTotal),
timedomain("Main time domain", ts, true) 
{
// 	StreamLogWriter w(std::cout);
// 	Logger log('C');
// 	log.set(w);
// 	w.show(LogLevel::INFO);
	
	getElmos.setName("getElmos");
	demuxEncoders.setName("demuxEncoders");
	demuxVelocities.setName("demuxVelocities");
	constantDigitalOut.setName("constantDigitalOut");
	constantDigitalOut1.setName("constantDigitalOut1");
	constantTargetTorque0.setName("constantTargetTorque0");
	muxDigitalOut.setName("muxDigitalOut");
	velocityChecker.setName("velocityChecker");
	timingPerformanceTester.setName("timingPerformanceTester");
	printNumber.setName("printNumber");
	setElmos.setName("setElmos");

	// Inputs
	demuxEncoders.getIn().connect(getElmos.getOutPosition());
	demuxVelocities.getIn().connect(getElmos.getOutVelocity());
	demuxTorque.getIn().connect(getElmos.getOutTorque());
	muxDigitalOut.getIn(0).connect(constantDigitalOut.getOut());
	muxDigitalOut.getIn(1).connect(constantDigitalOut1.getOut());
	
	// Internal logic
	timingPerformanceTester.getIn().connect(demuxTorque.getOut(0));
	
	// Signal checker
	velocityChecker.getIn().connect(demuxVelocities.getOut(0));
	
	// Outputs
	muxDigitalOut.getIn(0).connect(constantDigitalOut.getOut());
	muxDigitalOut.getIn(1).connect(constantDigitalOut1.getOut());
	muxTorqueOut.getIn(0).connect(timingPerformanceTester.getOut());
	muxTorqueOut.getIn(1).connect(constantTargetTorque0.getOut());
	printNumber.getIn().connect(demuxEncoders.getOut(0));
	setElmos.getInDigitalOutput().connect(muxDigitalOut.getOut());
	setElmos.getInTargetTorque().connect(muxTorqueOut.getOut());

	// Timedomains
	timedomain.addBlock(getElmos);
	timedomain.addBlock(demuxEncoders);
	timedomain.addBlock(demuxVelocities);
	timedomain.addBlock(demuxTorque);
	timedomain.addBlock(constantDigitalOut);
	timedomain.addBlock(constantDigitalOut1);
	timedomain.addBlock(constantTargetTorque0);
	
	timedomain.addBlock(timingPerformanceTester);
	timedomain.addBlock(velocityChecker);
	
	timedomain.addBlock(muxDigitalOut);
	timedomain.addBlock(muxTorqueOut);
	timedomain.addBlock(printNumber);
	timedomain.addBlock(setElmos);
	
	
	
	eeros::task::Periodic periodic("control system", ts, timedomain);  // create a periodic with a given period 
	// the periodic will run a given time domain
	
	periodic.monitors.push_back([&](eeros::PeriodicCounter &c, Logger &log) {
		static int ticks = 0;
		if (++ticks < 1000) return;
		ticks = 0;
		log.info() << "period max: " << c.period.max << ", run mean: " << c.run.mean;
// 		std::cout << "jitter max: " << c.jitter.max << ", run mean: " << c.run.mean << std::endl;
// 		std::cout << "period max: " << c.period.max << ", run max: " << c.run.max << std::endl;
		ticks = 0;
		c.reset();
	});
	
	eeros::Executor::instance().add(periodic);
	
	
// 	eeros::Executor::instance().add(timedomain);
}

void MyControlSystem::enableMonitoring()
{
	printNumber.enable();
}

void MyControlSystem::disableMonitoring()
{
	printNumber.disable();
}


MyControlSystem::~MyControlSystem() { }
