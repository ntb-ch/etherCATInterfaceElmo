#ifndef EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_
#define EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_


// #include "../etherCatInterface/EtherCATInterfaceElmo.hpp"
#include <EtherCATInterfaceElmo.hpp>

// #include <eeros/logger/StreamLogWriter.hpp>

// #include "MySafetyProperties.hpp"
// #include "MyControlSystem.hpp"
// #include "MainSequence.hpp"



namespace global {
	
// 	class MainSequence;
	
	static constexpr int numberOfDrivesTotal = 2;
	static constexpr int byteSizePerSlave = 32;
	
// 	eeros::logger::Logger* log;
// 	ecmasterlib::EcMasterlibMain* etherCATStackPtr;
// 	etherCATInterface::EtherCATInterfaceElmo* elmoDrivesPtr;
// 	MyControlSystem* CS;
// 	MySafetyProperties* safetyProperties;
// 	eeros::safety::SafetySystem* SS;
// 	eeros::sequencer::Sequencer* sequencer;
// 	MainSequence* mainSequence;
}

#endif	// EEROS_ETHERCAT_EXAMPLE_GLOBAL_CHOFING_HPP_
