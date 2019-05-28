#ifndef ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_
#define ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_

#include <EtherCATInterfaceBase_config.hpp>

namespace etherCATInterface {
	
	// Configurations offsets in PDOs (ENI files for EtherCAT network)
	// //////////////////////////////////////////
    static constexpr int numberOfDrives = 2;
	static constexpr int bytesPerSlaveTx = 32;    // eni.xml --> <EtherCATConfig> <Config> <ProcessImage> <Inputs> <ByteSize>
	static constexpr int bytesPerSlaveRx = 32;    // eni.xml --> <EtherCATConfig> <Config> <ProcessImage> <Inputs> <ByteSize>

	// Input Offsets (io) PDOs [in Byte]
	// Elmo: GoldLine "EtherCAT Application Manual.pdf"     p. 23
	static constexpr int io_statusWord					= 0;	// 0x6041
	static constexpr int io_modeOfOperationDisplay		= 2; 	// 0X6061
	static constexpr int io_postionDemand_UU			= -3;	// 0X6062
	static constexpr int io_actualPosition_counts		= -4;	// 0X6063
	static constexpr int io_positionActualValue			= 4; 	// 0X6064	used for getPosition()
	static constexpr int io_velocitySensorActualValue	= -6;	// 0X6069
	static constexpr int io_velocityDemand				= -7;	// 0X606B
	static constexpr int io_velocityActualValue			= 8; 	// 0X606C	used for getVelocity
	static constexpr int io_torqueDemandValue			= -9;	// 0X6074
	static constexpr int io_torqueActualValue			= 12;	// 0X6077	used for getTorque
	static constexpr int io_touchProbeStatus			= 14;	// 0X60B9
	static constexpr int io_touchProbePos1Positive		= 16;	// 0X60BA
	static constexpr int io_touchProbePos1Negative		= 20;	// 0x60BB
	static constexpr int io_touchProbePos2Positive		= 24;	// 0x60BC
	static constexpr int io_DCLinkCircuitVoltage		= -15;	// 0x6079
	static constexpr int io_positionFollowingError		= -16;	// 0x60F4
	static constexpr int io_controlEffort				= -17;	// 0x60FA
	static constexpr int io_positionDemandValue_cnt		= -18;	// 0x60FC
	static constexpr int io_digitalInputs				= 28;	// 0x60FD
	static constexpr int io_analogInput					= -19;	// 0x2205
	static constexpr int io_auxilaryPositionActualValue	= -20;	// 0x20A0
	static constexpr int io_currentActualValue			= -21;	// 0x6078
	
		
	// Output Offsets (oo) PDOs (output from master) [in Byte]
	// Elmo: GoldLine "EtherCAT Application Manual.pdf"     p. 21
	static constexpr int oo_controlWord				= 0;		// 0x6040
	static constexpr int oo_modeOfOperation			= 2;		// 0x6060
	static constexpr int oo_targetTorque			= 4;		// 0x6071
	static constexpr int oo_maxTorque				= -1;		// 0x6072
	static constexpr int oo_maxCurrent				= -1;		// 0x6073
	static constexpr int oo_targetPosition			= 6;		// 0x607A
	static constexpr int oo_maxProfileVelocity		= -1;		// 0x607F
	static constexpr int oo_profileVelocity			= -1;		// 0x6081
	static constexpr int oo_endVelocity				= -1;		// 0x6082
	static constexpr int oo_profileAcceleration		= -1;		// 0x6083
	static constexpr int oo_profileDeceleration		= -1;		// 0x6084
	static constexpr int oo_torqueSlope				= -1;		// 0x6087
	static constexpr int oo_positionOffset			= -1;		// 0x60B0
	static constexpr int oo_velocityOffset			= -1;		// 0x60B1
	static constexpr int oo_torqueOffset			= -1;		// 0x60B2
	static constexpr int oo_touchProbeFunction		= 10;		// 0x60B8
	static constexpr int oo_interpolatedDataRecord_1		= -1;		// 0x60C1:1
	static constexpr int oo_interpolatedDataRecord_2		= -1;		// 0x60C1:2
	static constexpr int oo_targetVelocity			= 12;		// 0x60FF
	static constexpr int oo_digitalOutput			= 16;		// 0x60FE:1
	static constexpr int oo_polarity				= -1;		// 0x607F
	
	static constexpr int oo_gainSchedlingManualIndex = -1;
	
	
	
	// Configurations for Elmo drives
	// //////////////////////////////
	
	// Elmo: GoldLine "CAN DS-402 Implementation Guide"		p.56
	enum driveModeOfOperation_ELMO {
		CANEncoderMode,
		profilePosition,
		profileVelocity,
		profileTorque,
		homing,
		interpolatedPosition,
		cyclicSynchronousPosition,
		cyclicSynchronousVelocity,
		cyclicSynchronousTorque
	};
	
	// drive mode of operations value (dmoov)
	static constexpr int16_t dmoov_CANEncoderMode				= -3;
	static constexpr int16_t dmoov_profilePosition				= 1;
	static constexpr int16_t dmoov_profileVelocity				= 3;
	static constexpr int16_t dmoov_profileTorque				= 4;
	static constexpr int16_t dmoov_homing						= 6;
	static constexpr int16_t dmoov_interpolatedPosition			= 7;
	static constexpr int16_t dmoov_cyclicSynchronousPosition	= 8;
	static constexpr int16_t dmoov_cyclicSynchronousVelocity	= 9;
	static constexpr int16_t dmoov_cyclicSynchronousTorque		= 10;


	// Elmo: GoldLine "CAN DS-402 Implementation Guide"		p.43
	enum controlWordCommand_ELMO {
	shutdown,			//0x06
	switchOn,			//0X07
	switchOnAndEnable,	//0x0F
	disableVoltage,		//0x00
	quickStop,			//0x02
	disableOperation,	//0x07
	enableOperation,	//0x0F
	faultReset			//0X80
	};
	  
	// Control World Commands (cwc)
	static constexpr uint16_t cwc_shutdown			= 0x06;
	static constexpr uint16_t cwc_switchOn			= 0x07;
	static constexpr uint16_t cwc_switchOnAndEnable	= 0x0F;
	static constexpr uint16_t cwc_disableVoltage	= 0x00;
	static constexpr uint16_t cwc_quickStop			= 0x02;
	static constexpr uint16_t cwc_disableOperation	= 0x07;
	static constexpr uint16_t cwc_enableOperation	= 0x0F;
	static constexpr uint16_t cwc_faultReset		= 0x80;
	  
	
	// Elmo: GoldLine "CAN DS-402 Implementation Guide"		p.46
	enum driveStatus_ELMO {
		notReadyToSwitchOn,
		switchOnDisabled,
		readyToSwitchOn,
		switchedOn,
		operationEnabled,
		quickStopActive,
		faultReactionactive,
		fault
	};

	static constexpr uint16_t notReadyToSwitchOnValue =		0x0000;
	static constexpr uint16_t notReadyToSwitchOnMask =		0x004f;
	static constexpr uint16_t switchOnDisabledValue =		0x0040;
	static constexpr uint16_t switchOnDisabledMask =		0x004f;
	static constexpr uint16_t readyToSwitchOnValue =		0x0021;
	static constexpr uint16_t readyToSwitchOnMask = 		0x006f;
	static constexpr uint16_t switchedOnValue =				0x0023;
	static constexpr uint16_t switchedOnMask =				0x006f;
	static constexpr uint16_t operationEnabledValue = 		0x0027;
	static constexpr uint16_t operationEnabledMask = 		0x006f;
	static constexpr uint16_t quickStopActiveValue =		0x0007;
	static constexpr uint16_t quickStopActiveMask =			0x006f;
	static constexpr uint16_t faultReactionActiveValue =	0x000f;
	static constexpr uint16_t faultReactionActiveMask =		0x004f;
	static constexpr uint16_t faultValue =					0x0008;
	static constexpr uint16_t faultMask =					0x0048;
	
	
	// Touch probe status
	enum touchProbeStateEnum_ELMO {
		  resetting,
		  enablingProbe,
		  enablingSampling,
		  enabled
	  };

	static constexpr uint16_t touchProbe1EnabledValue				= 0x0001;
	static constexpr uint16_t touchProbe1EnabledMask				= 0x0001;
	static constexpr uint16_t touchProbe1PositiveEdgeStoredValue	= 0x0002;
	static constexpr uint16_t touchProbe1PositiveEdgeStoredMask		= 0x0002;
	static constexpr uint16_t touchProbe1NegativeEdgeStoredValue	= 0x0004;
	static constexpr uint16_t touchProbe1NegativeEdgeStoredMask		= 0x0004;
	static constexpr uint16_t touchProbe2EnabledValue				= 0x0100;
	static constexpr uint16_t touchProbe2EnabledMask				= 0x0100;
	static constexpr uint16_t touchProbe2PositiveEdgeStoredValue	= 0x0200;
	static constexpr uint16_t touchProbe2PositiveEdgeStoredMask		= 0x0200;
	static constexpr uint16_t touchProbe2NegativeEdgeStoredValue	= 0x0400;
	static constexpr uint16_t touchProbe2NegativeEdgeStoredMask		= 0x0400;
	
	enum touchProbeFunctionEnum_ELMO {
		switchOffTouchProbe1,
		enableTouchProbe1,
		triggerWithTouchProbe1Input,
		triggerWitchZeroImpulsSignalOrPositioinEncoder1,
		touchProbeSourceAsDefinedInObject0x60D0SI01,
		switchOffSamplingAtThePositiveEdgeOfTouchProbe1,
		enableSamplingAtThePositiveEdgeOfTouchProbe1,
		switchOffSamplingAtTheNegativeEdgeOfTouchProbe1,
		enableSamplingAtTheNegativeEdgeOfTouchProbe1,
		switchOffTouchProbe2,
		enableTouchProbe2,
		triggerWithTouchProbe2Input,
		triggerWitchZeroImpulsSignalOrPositioinEncoder2,
		touchProbeSourceAsDefinedInObject0x60D0SI02,
		switchOffSamplingAtThePositiveEdgeOfTouchProbe2,
		enableSamplingAtThePositiveEdgeOfTouchProbe2,
		switchOffSamplingAtTheNegativeEdgeOfTouchProbe2,
		enableSamplingAtTheNegativeEdgeOfTouchProbe2
	};
		
		

	// switch to different set of gains for velocity/torque 
	static constexpr int gainSchedulingManualIndex_position =	-1;		// chair 1
	static constexpr int gainSchedulingManualIndex_velocity =	-1;		// chair 1
	static constexpr int gainSchedulingManualIndex_torque =		-1;		// chair 2
	static constexpr int gainSchedulingManualIndex_homing =		-1;		// chair 2
}

#endif // ETHERCAT_INTERFACE_ELMO_CONFIG_HPP_
