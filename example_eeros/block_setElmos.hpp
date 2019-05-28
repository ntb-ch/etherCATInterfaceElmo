#ifndef BLOCK_SET_ELMOS_HPP
#define BLOCK_SET_ELMOS_HPP

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Signal.hpp>
#include <eeros/math/Matrix.hpp>
#include <eeros/core/System.hpp>

#include <EtherCATInterfaceElmo.hpp>
#include "globalConfig.hpp"


using namespace eeros::control;
using namespace etherCATInterface;

//TODO numberOfDrivesTotal
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint16_t >			typeControlWord;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint8_t >			typeModeOfOperation;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, double >			typeTargetTorque;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int16_t >			typeMaxTorque;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int16_t >			typeMaxCurrent;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typeTargetPosition;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeMaxProfileVelocity;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeProfileVelocity;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeEndVelocity;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeProfileAcceleration;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeProfileDeceleration;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t  >		typeTorqueSlope;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typePositionOffset;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typeVelocityOffset;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int16_t >			typeTorqueOffset;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint16_t >			typeTouchProbeFunction;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typeInterpolatedDataRecord_1;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typeInterpolatedDataRecord_2;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, int32_t >			typeTargetVelocity;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint32_t >			typeDigitalOutput;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint8_t >			typePolarity;
typedef eeros::math::Matrix< global::numberOfDrivesTotal, 1, uint16_t >			typeGainSchedlingManualIndex;


class block_setElmos : public Block {
public:
	block_setElmos(EtherCATInterfaceElmo& elmoDrives, int numberOfDrivesTotal) :
		elmoDrives(elmoDrives),
		numberOfDrivesTotal(numberOfDrivesTotal),
		ecmasterlibWaitsForThisBlock(false),
		
		setControlWordByCS(false ),
		setModeOfOperationByCS(false),
		setTargetTorqueByCS(false),
		setMaxTorqueByCS(false),
		setMaxCurrentByCS(false),
		setTargetPositionByCS(false),
		setMaxProfileVelocityByCS(false),
		setProfileVelocityByCS(false),
		setEndVelocityByCS(false),
		setProfileAccelerationByCS(false),
		setProfileDecelerationByCS(false),
		setTorqueSlopeByCS(false),
		setPositionOffsetByCS(false),
		setVelocityOffsetByCS(false),
		setTorqueOffsetByCS(false),
		setTouchProbeFunctionByCS(false),
		setInterpolatedDataRecord_1ByCS(false),
		setInterpolatedDataRecord_2ByCS(false),
		setTargetVelocityByCS(false),
		setDigitalOutputByCS(true),
		setPolarityByCS(false),
		setGainSchedlingManualIndexByCS(false)
	{}
	
	virtual void run() {
		// set PDO values from EtherCAT using EtherCATInterfaceElmo
		//  if the output value is not sent by PDO (Interface_config: oo_... < 0) the variable will not be processed

		for ( int i = 0; i < numberOfDrivesTotal; i++ ) {
 			if( etherCATInterface::oo_controlWord >= 0 && setControlWordByCS )
 				elmoDrives.ll_setControlWord(i, inControlWord.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_modeOfOperation >= 0 && setModeOfOperationByCS )
 				elmoDrives.ll_setModeOfOperation(i, inModeOfOperation.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_targetTorque >= 0 && setTargetTorqueByCS )
 				elmoDrives.ll_setTargetTorque(i, static_cast<uint16_t>( inTargetTorque.getSignal().getValue()(i) ));
 			if( etherCATInterface::oo_maxTorque >= 0 && setMaxTorqueByCS )
 				elmoDrives.ll_setMaxTorque(i, inMaxTorque.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_maxCurrent >= 0 && setMaxCurrentByCS )
 				elmoDrives.ll_setMaxCurrent(i, inMaxCurrent.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_targetPosition >= 0 && setTargetPositionByCS )
 				elmoDrives.ll_setTargetPosition(i, inTargetPosition.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_maxProfileVelocity >= 0 && setMaxProfileVelocityByCS )
 				elmoDrives.ll_setMaxProfileVelocity(i, inMaxProfileVelocity.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_profileVelocity >= 0 && setProfileVelocityByCS )
 				elmoDrives.ll_setProfileVelocity(i, inProfileVelocity.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_endVelocity >= 0 && setEndVelocityByCS )
 				elmoDrives.ll_setEndVelocity(i, inEndVelocity.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_profileAcceleration >= 0 && setProfileAccelerationByCS )
 				elmoDrives.ll_setProfileAcceleration(i, inProfileAcceleration.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_profileDeceleration >= 0 && setProfileDecelerationByCS )
 				elmoDrives.ll_setProfileDeceleration(i, inProfileDeceleration.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_torqueSlope >= 0 && setTorqueSlopeByCS )
 				elmoDrives.ll_setTorqueSlope(i, inTorqueSlope.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_positionOffset >= 0 && setPositionOffsetByCS )
 				elmoDrives.ll_setPositionOffset(i, inPositionOffset.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_velocityOffset >= 0 && setVelocityOffsetByCS )
 				elmoDrives.ll_setVelocityOffset(i, inVelocityOffset.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_torqueOffset >= 0 && setTorqueOffsetByCS )
 				elmoDrives.ll_setTorqueOffset(i, inTorqueOffset.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_touchProbeFunction >= 0 && setTouchProbeFunctionByCS )
 				elmoDrives.ll_setTouchProbeFunction(i, inTouchProbeFunction.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_interpolatedDataRecord_1 >= 0 && setInterpolatedDataRecord_1ByCS )
 				elmoDrives.ll_setInterpolatedDataRecord1(i, inInterpolatedDataRecord_1.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_interpolatedDataRecord_2 >= 0 && setInterpolatedDataRecord_2ByCS )
 				elmoDrives.ll_setInterpolatedDataRecord2(i, inInterpolatedDataRecord_2.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_targetVelocity >= 0 && setTargetVelocityByCS )
 				elmoDrives.ll_setTargetVelocity(i, inTargetVelocity.getSignal().getValue()(i) );
			if( etherCATInterface::oo_digitalOutput >= 0 && setDigitalOutputByCS )
				elmoDrives.ll_setDigitalOutput(i, inDigitalOutput.getSignal().getValue()(i) );
 			if( etherCATInterface::oo_polarity >= 0 && setPolarityByCS )
 				elmoDrives.ll_setPolarity(i, inPolarity.getSignal().getValue()(i) );
			
//  			if( etherCATInterface::oo_gainSchedlingManualIndex  >= 0 && setGainSchedlingManualIndexByCS )
//  				elmoDrives.ll_setGainSchedlingManualIndex (i, inGainSchedlingManualIndex .getSignal().getValue()(i) );
			
			
			
// 			if (!ecmasterlibWaitsForThisBlock) {
// 				elmoDrives.getEtherCATStack()->getInstance()->startWaitingForEeros();
// 				ecmasterlibWaitsForThisBlock = true;
// 			}
// 			elmoDrives.getEtherCATStack()->getInstance()->getConditionalVariable2()->notify_one();
		}
	}
	
	
	// Defining getIn...() methods
		virtual eeros::control::Input<typeControlWord>& getInControlWord() {
			return inControlWord;
		}
		virtual eeros::control::Input<typeModeOfOperation>& getInModeOfOperation() {
			return inModeOfOperation;
		}
		virtual eeros::control::Input<typeTargetTorque>& getInTargetTorque() {
			return inTargetTorque;
		}
		virtual eeros::control::Input<typeMaxTorque>& getInMaxTorque() {
			return inMaxTorque;
		}
		virtual eeros::control::Input<typeMaxCurrent>& getInMaxCurrent() {
			return inMaxCurrent;
		}
		virtual eeros::control::Input<typeTargetPosition>& getInTargetPosition() {
			return inTargetPosition;
		}
		virtual eeros::control::Input<typeMaxProfileVelocity>& getInMaxProfileVelocity() {
			return inMaxProfileVelocity;
		}
		virtual eeros::control::Input<typeProfileVelocity>& getInProfileVelocity() {
			return inProfileVelocity;
		}
		virtual eeros::control::Input<typeEndVelocity>& getInEndVelocity() {
			return inEndVelocity;
		}
		virtual eeros::control::Input<typeProfileAcceleration>& getInProfileAcceleration() {
			return inProfileAcceleration;
		}
		virtual eeros::control::Input<typeProfileDeceleration>& getInProfileDeceleration() {
			return inProfileDeceleration;
		}
		virtual eeros::control::Input<typeTorqueSlope>& getInTorqueSlope() {
			return inTorqueSlope;
		}
		virtual eeros::control::Input<typePositionOffset>& getInPositionOffset() {
			return inPositionOffset;
		}
		virtual eeros::control::Input<typeVelocityOffset>& getInVelocityOffset() {
			return inVelocityOffset;
		}
		virtual eeros::control::Input<typeTorqueOffset>& getInTorqueOffset() {
			return inTorqueOffset;
		}
		virtual eeros::control::Input<typeTouchProbeFunction>& getInTouchProbeFunction() {
			return inTouchProbeFunction;
		}
		virtual eeros::control::Input<typeInterpolatedDataRecord_1>& getInInterpolatedDataRecord_1() {
			return inInterpolatedDataRecord_1;
		}
		virtual eeros::control::Input<typeInterpolatedDataRecord_2>& getInInterpolatedDataRecord_2() {
			return inInterpolatedDataRecord_2;
		}
		virtual eeros::control::Input<typeTargetVelocity>& getInTargetVelocity() {
			return inTargetVelocity;
		}
		virtual eeros::control::Input<typeDigitalOutput>& getInDigitalOutput() {
			return inDigitalOutput;
		}
		virtual eeros::control::Input<typePolarity>& getInPolarity() {
			return inPolarity;
		}
		virtual eeros::control::Input<typeGainSchedlingManualIndex>& getInGainSchedlingManualIndex() {
			return inGainSchedlingManualIndex;
		}
	
		
		bool setControlWordByCS;
		bool setModeOfOperationByCS;
		bool setTargetTorqueByCS;
		bool setMaxTorqueByCS;
		bool setMaxCurrentByCS;
		bool setTargetPositionByCS;
		bool setMaxProfileVelocityByCS;
		bool setProfileVelocityByCS;
		bool setEndVelocityByCS;
		bool setProfileAccelerationByCS;
		bool setProfileDecelerationByCS;
		bool setTorqueSlopeByCS;
		bool setPositionOffsetByCS;
		bool setVelocityOffsetByCS;
		bool setTorqueOffsetByCS;
		bool setTouchProbeFunctionByCS;
		bool setInterpolatedDataRecord_1ByCS;
		bool setInterpolatedDataRecord_2ByCS;
		bool setTargetVelocityByCS;
		bool setDigitalOutputByCS;
		bool setPolarityByCS;
		bool setGainSchedlingManualIndexByCS;
	
	protected:
//TODO numberOfDrivesTotal
		eeros::control::Input< typeControlWord >			inControlWord;
		eeros::control::Input< typeModeOfOperation >		inModeOfOperation;
		eeros::control::Input< typeTargetTorque >			inTargetTorque;
		eeros::control::Input< typeMaxTorque >				inMaxTorque;
		eeros::control::Input< typeMaxCurrent >				inMaxCurrent;
		eeros::control::Input< typeTargetPosition >			inTargetPosition;
		eeros::control::Input< typeMaxProfileVelocity >		inMaxProfileVelocity;
		eeros::control::Input< typeProfileVelocity >		inProfileVelocity;
		eeros::control::Input< typeEndVelocity >			inEndVelocity;
		eeros::control::Input< typeProfileAcceleration >		inProfileAcceleration;
		eeros::control::Input< typeProfileDeceleration >		inProfileDeceleration;
		eeros::control::Input< typeTorqueSlope >				inTorqueSlope;
		eeros::control::Input< typePositionOffset >				inPositionOffset;
		eeros::control::Input< typeVelocityOffset >				inVelocityOffset;
		eeros::control::Input< typeTorqueOffset >				inTorqueOffset;
		eeros::control::Input< typeTouchProbeFunction >			inTouchProbeFunction;
		eeros::control::Input< typeInterpolatedDataRecord_1 >	inInterpolatedDataRecord_1;
		eeros::control::Input< typeInterpolatedDataRecord_2 >	inInterpolatedDataRecord_2;
		eeros::control::Input< typeTargetVelocity >				inTargetVelocity;
		eeros::control::Input< typeDigitalOutput >				inDigitalOutput;
		eeros::control::Input< typePolarity >					inPolarity;
		eeros::control::Input< typeGainSchedlingManualIndex >	inGainSchedlingManualIndex;
		

		int numberOfDrivesTotal;
		EtherCATInterfaceElmo& elmoDrives;
		bool ecmasterlibWaitsForThisBlock;
};

#endif /* BLOCK_SET_ELMOS_HPP */
