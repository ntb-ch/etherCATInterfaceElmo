#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/DeMux.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/SignalChecker.hpp>

#include <EtherCATInterfaceElmo.hpp>

#include "block_getElmos.hpp"
#include "block_setElmos.hpp"
#include "block_printNumber.hpp"
#include "block_timingPerformanceTester.hpp"

using namespace etherCATInterface;

class MyControlSystem {

public:
	MyControlSystem(double ts, EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal, Logger& log);
	~MyControlSystem();
	
	
	//TODO numberOfDrivesTotal
	// Inputs
	block_getElmos getElmos;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxEncoders;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxVelocities;
	eeros::control::DeMux<global::numberOfDrivesTotal, double> demuxTorque;
	eeros::control::Constant<uint32_t> constantDigitalOut;
	eeros::control::Constant<uint32_t> constantDigitalOut1;
	eeros::control::Constant<double> constantTargetTorque0;
	
	
	// Internal logic
	block_timingPerformanceTester<double> timingPerformanceTester;
	
	// SignalChecker
	eeros::control::SignalChecker< double > velocityChecker;
	
	
	// Outputs
	eeros::control::Mux<global::numberOfDrivesTotal, uint32_t> muxDigitalOut;
	eeros::control::Mux<global::numberOfDrivesTotal, double> muxTorqueOut;
	block_printNumber<double> printNumber;
	block_setElmos setElmos;
	
	
	// Methods
	void enableMonitoring();
	void disableMonitoring();

	eeros::control::TimeDomain timedomain;
	
	
private:
	EtherCATInterfaceElmo& elmoDrives;
	int numberOfDrivesTotal;
	Logger& log;
};

#endif // CONTROLSYSTEM_HPP_
