#ifndef SAFETYPROPERTIES_HPP_
#define SAFETYPROPERTIES_HPP_


#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>

#include <EtherCATInterfaceElmo.hpp>

#include "MyControlSystem.hpp"

class MySafetyProperties : public eeros::safety::SafetyProperties {
	
public:
	MySafetyProperties(MyControlSystem& CS, EtherCATInterfaceElmo& elmoDrives, double ts);
	virtual ~MySafetyProperties();
	
	// Name all levels
// 	eeros::safety::SafetyLevel slFault;
	eeros::safety::SafetyLevel slOff;
	eeros::safety::SafetyLevel slEmergency;
	eeros::safety::SafetyLevel slDrivesDisabled;
	eeros::safety::SafetyLevel slDrivesEnabled;
	eeros::safety::SafetyLevel slHoming;
	eeros::safety::SafetyLevel slMoving;
	
// 	eeros::safety::SafetyLevel slSystemOn;
// 	eeros::safety::SafetyLevel slStartingControl;
	
	// Define all possible events
// 	eeros::safety::SafetyEvent fault;
	eeros::safety::SafetyEvent switchOff;
	eeros::safety::SafetyEvent doEmergency;
	eeros::safety::SafetyEvent recoverEmergency;
	eeros::safety::SafetyEvent disableDrives;
	eeros::safety::SafetyEvent enableDrives;
	eeros::safety::SafetyEvent doHoming;
	eeros::safety::SafetyEvent startMoving;
	
	
protected:
	// critical outputs
	eeros::hal::Output<bool>* safetyTorqueOff;
	
	// critical inputs
	eeros::hal::Input<bool>* emergency;
// 	eeros::hal::Input<bool>* ready;
		
	MyControlSystem& CS;
	EtherCATInterfaceElmo& elmoDrives;
	Logger log;
};

#endif // SAFETYPROPERTIES_HPP_
