#include "MySafetyProperties.hpp"
#include "MyControlSystem.hpp"

#include <eeros/safety/InputAction.hpp>
#include <eeros/safety/OutputAction.hpp>
#include <eeros/core/Executor.hpp>

#include <unistd.h>
// #include <iostream>
// #include <vector>
#include <initializer_list>

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::safety;


// MySafetyProperties::MySafetyProperties(MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, double dt) : 
MySafetyProperties::MySafetyProperties(MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, double dt) : 
	CS(CS),
	elmoDrives(elmoDrives),
	log('S'),
	// ############ Define Levels ############
	slOff("Software is off"),
	slEmergency("Emergency state"),
	slDrivesDisabled("Drives are disabled"),
	slDrivesEnabled("Drives are enabled and ready to move"),
	slHoming("Homing"),
	slMoving("Drives are moving"),
	
	switchOff("Switching off"),
	doEmergency("Emergency activated"),
	recoverEmergency("Recover from emergency"),
	disableDrives("Disabeling drives"),
	enableDrives("Enabeling drives"),
	doHoming("Start homing"),
	startMoving("Drives start moving")

// 	abort("abort")
	{
// 	HAL& hal = HAL::instance();

	// ############ Define critical outputs ############
// 	safetyTorqueOff = hal.getLogicOutput("safetyTorqueOff");
	
// 	criticalOutputs = { safetyTorqueOff };
	
	// ############ Define critical inputs ############
// 	emergency = hal.getLogicInput("emergency");
// 	ready = hal.getLogicInput("readySig1");
	
// 	criticalInputs = { emergency, ready };
	
	addLevel(slOff);
	addLevel(slEmergency);
	addLevel(slDrivesDisabled);
	addLevel(slDrivesEnabled);
	addLevel(slHoming);
	addLevel(slMoving);
// 	addLevel(slMoving);
// 	addLevel(slFault);
	
	slEmergency			.addEvent(recoverEmergency,	slDrivesDisabled,		kPublicEvent  );
	slDrivesDisabled	.addEvent(enableDrives,		slDrivesEnabled,		kPublicEvent  );
	slDrivesEnabled		.addEvent(doHoming,			slHoming,				kPublicEvent  );
	slDrivesEnabled		.addEvent(startMoving,		slMoving,				kPublicEvent  );
	slHoming			.addEvent(startMoving,		slMoving,				kPublicEvent  );
	
	
	// Add events to multiple levels
	addEventToLevelAndAbove(slOff,				doEmergency,		slEmergency,		kPublicEvent);
	addEventToLevelAndAbove(slEmergency,		switchOff,			slOff,				kPublicEvent);
	addEventToLevelAndAbove(slDrivesEnabled,	disableDrives,		slDrivesDisabled,	kPublicEvent);
// 	addEventToLevelAndAbove(slMoving, doEmergency, slEmergency, kPublicEvent);
// 	addEventToLevelAndAbove(slEmergency, abort, slStoppingControl, kPublicEvent);
		
	
	// ############ Define input states and events for all levels ############
// 	slOff			.setInputActions( { ignore(emergency), ignore(ready) });
// 	slEmergency		.setInputActions( { ignore(emergency), ignore(ready) });
// 	slSystemOn		.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slStartingControl	.setInputActions( { check(emergency, true , doEmergency), ignore(ready)});
// 	slStoppingControl	.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slPowerOn		.setInputActions( { check(emergency, true , doEmergency), ignore(ready) });
// 	slMoving		.setInputActions( { check(emergency, true , doEmergency), check(ready, true, doEmergency) });
	
// 	slOff			.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slEmergency		.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slSystemOn		.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slStartingControl	.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slStoppingControl	.setOutputActions( { set(safetyTorqueOff, false) } );;
// 	slPowerOn		.setOutputActions( { set(safetyTorqueOff, true) } );;
// 	slMoving		.setOutputActions( { set(safetyTorqueOff, true) } );;
	
	
	// Define and add level functions
	/////////////////////////////////////////////////////////////// 
	slOff.setLevelAction([&](SafetyContext* privateContext) {
		elmoDrives.disableAllDrives();
		sleep(1);
		Executor::stop();
	});
	
	slEmergency.setLevelAction([&](SafetyContext* privateContext) {
		if (slEmergency.getNofActivations() == 1) { log.info() << "slEmergency"; };
		//quick stop
// 		int driveNumber;
		for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++ ) {
			if ( elmoDrives.getIsDriveEnabled(driveNumber) ) {
				// quickStop is only available if drive is enabled
				elmoDrives.setControlWord(driveNumber, quickStop);
			}
			else {
				//TODO set DIO for STO
			}
			elmoDrives.disableDrive(driveNumber);
		}
	});
	
	slDrivesDisabled.setLevelAction([&](SafetyContext* privateContext) {
		for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++ ) {
            elmoDrives.disableDrive(driveNumber);
        }
// 		elmoDrives.disableAllDrives();
	});
	
	slDrivesEnabled.setLevelAction([&](SafetyContext* privateContext) {
// 		elmoDrives.enableDrive(0);
// 		elmoDrives.enableAllDrives();
		// check max speed and acceleration with signal checker
	});
	
	slHoming.setLevelAction([&](SafetyContext* privateContext) {
		// check max speed and acceleration with signal checker
	});
	
	slMoving.setLevelAction([&](SafetyContext* privateContext) {
		// check max speed and acceleration with signal checker
	});
	
	
	// Define entry level
	setEntryLevel(slDrivesDisabled);
	
	exitFunction = ([&](SafetyContext* privateContext){
		privateContext->triggerEvent(switchOff);
	});
	
}

MySafetyProperties::~MySafetyProperties() { }
