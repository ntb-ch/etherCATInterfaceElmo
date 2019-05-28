#ifndef ETHERCAT_INTERFACE_ELMO_HPP_
#define ETHERCAT_INTERFACE_ELMO_HPP_

#include <list>

#include <EtherCATInterfaceBase.hpp>
#include "EtherCATInterfaceElmo_config.hpp"


namespace etherCATInterface {
	class EtherCATInterfaceElmo : public EtherCATInterfaceBase {
	public:
		EtherCATInterfaceElmo(ecmasterlib::EcMasterlibMain* etherCATStack);
		
		// advanced set functions
		//  call repeatedly until the method returns 'true'
		bool initAllDrives();
		bool recoverAllDrivesFromFault();
		bool switchOnAllDrives();
		bool switchOnDrive(int driveNumber);
		bool enableAllDrives();
		void disableAllDrives();
		void setControlWord(int driveNumber, controlWordCommand_ELMO word);
		
		// advanced get functions
		ecmasterlib::EcMasterlibMain* getEtherCATStack() { return etherCATStack; };
		bool isAllDrivesReady();
		bool isAllDrivesEnabled();
		// getPos-methodes prevent overflows with 32-bit integers
		int64_t getPosition(int driveNumber);
		int64_t getPositionAux(int driveNumber);
		// gain scheduling functions
		void disableVelocityControl(int driveNumber);
		void enableVelocityControl(int driveNumber);
		
		// basic functions
		void disableDrive(int driveNumber);
		bool enableDrive(int driveNumber);
		void setModeOfOperation(int driveNumber, driveModeOfOperation_ELMO mode, bool scheduleGainIndex = false);
		bool getIsDriveEnabled(int driveNumber);
		bool getIsDriveReady(int driveNumber);
		bool getIsDriveSwitchedOn(int driveNumber);
		driveModeOfOperation_ELMO getDriveModeElmo(int driveNumber);
		bool checkDriveStatus(int driveNumber, driveStatus_ELMO driveStatusToCheck);
		driveStatus_ELMO getDriveStatusElmo(int driveNumber);
		std::string getDriveStatusStringElmo(int driveNumber);
		
		
		// index puls
		bool enableCapturingIndexPulse(std::vector<int> driveNumbers);
		bool enableCapturingIndexPulse(std::vector<int> driveNumbers, std::vector<int> touchProbes);
		bool enableCapturingIndexPulse(int driveNumber, int touchProbe=1);
		bool disableCapturingIndexPulse(int driveNumber, int touchProbe=1);
		// use 'setOffsetAtIndexPos' after capturing an index pulse to set the position at the index pulse to 0 or an arbitrary value
		void setOffsetAtIndexPos(std::vector<int> driveNumbers, std::vector<int> offsets, bool isAuxPos=false);
		void setOffsetAtIndexPos(std::vector<int> driveNumbers, std::vector<int> offsets, bool isAuxPos, std::vector<int> touchProbes);
		void setOffsetAtIndexPos(int driveNumber, int offset=0, bool isAuxPos=false, int touchProbe=1);
		void setPosOffset(int driveNumber, int32_t offset);
		void setPosAuxOffset(int driveNumber, int32_t offset);
		void setTouchProbeFunction(int driveNumber, touchProbeFunctionEnum_ELMO function);
		
		bool getTouchProbeIsEnabled(int driveNumber, int touchProbe=1);
		bool getIndexPulseIsCaptured(int driveNumber, int touchProbe=1);	// both pulses are captured
		bool getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe=1);
		bool getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe=1);
		int32_t getCapturedPosition(int driveNumber, int touchProbe=1);
		int32_t getCapturedPositionPositivePulse(int driveNumber, int touchProbe=1);
		int32_t getCapturedPositionNegativePulse(int driveNumber, int touchProbe=1);
		
		
		// Low level get and set functions to get/write values from/to buffer array
		// ////////////////////////////////////////////////////////////////////
		
		// low level set functions:
		void ll_setControlWord(int driveNumber, uint16_t controlWord);
		void ll_setModeOfOperation(int driveNumber, int8_t modeOfOperation);
		void ll_setTargetTorque(int driveNumber, int16_t targetTorque);
		void ll_setMaxTorque(int driveNumber, int16_t maxTorque);
		void ll_setMaxCurrent(int driveNumber, int16_t maxCurrent);
		void ll_setTargetPosition(int driveNumber, int32_t targetPosition);
		void ll_setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity);
		void ll_setProfileVelocity(int driveNumber, uint32_t profileVelocity);
		void ll_setEndVelocity(int driveNumber, uint32_t endVelocity);
		void ll_setProfileAcceleration(int driveNumber, uint32_t profileAcceleration);
		void ll_setProfileDeceleration(int driveNumber, uint32_t profileDeceleration);
		void ll_setTorqueSlope(int driveNumber, uint32_t torqueSlope);
		void ll_setPositionOffset(int driveNumber, int32_t positionOffset);
		void ll_setVelocityOffset(int driveNumber, int32_t velocityOffset);
		void ll_setTorqueOffset(int driveNumber, int16_t torqueOffset);
		void ll_setTouchProbeFunction(int driveNumber, uint16_t touchProbeFunction);
		void ll_setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1);
		void ll_setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2);
		void ll_setTargetVelocity(int driveNumber, int32_t targetVelocity);
		void ll_setDigitalOutput(int driveNumber, uint32_t digitalOutput);
		void ll_setPolarity(int driveNumber, uint8_t polarity);
		void ll_setGainSchedulingManualIndex(int driveNumber, uint16_t index);
		
		
		// low level get functions:
		uint16_t ll_getStatusWord(int driveNumber);
		int8_t ll_getModeOfOperationDisplay(int driveNumber);
		int32_t ll_getPositionDemand_UU(int driveNumber);			// 0x6062 position demand; input internal pos controler
		int32_t ll_getActualPosition_counts(int driveNumber);		// 0x6063 positin raw in counts
		int32_t ll_getPositionActualValue(int driveNumber);			// 0x6064 position in user unit
		int32_t ll_getVelocitySensorActualValue(int driveNumber);	// 0x6069 velocity from encoder in counts/sec
		int32_t ll_getVelocityDemand(int driveNumber);
		int32_t ll_getVelocityActualValue(int driveNumber);			// 0x606c velocity in user units; used internaly for velocity controler
		int16_t ll_getTorqueDemandValue(int driveNumber);
		int16_t ll_getTorqueActualValue(int driveNumber);
		uint16_t ll_getTouchProbeStatus(int driveNumber);
		int32_t ll_getTouchProbePos1Positive(int driveNumber);
		int32_t ll_getTouchProbePos1Negative(int driveNumber);
		int32_t ll_getTouchProbePos2Positive(int driveNumber);
		uint32_t ll_getDCLinkCircuitVoltage(int driveNumber);
		int32_t ll_getPositionFollowingError(int driveNumber);
		int32_t ll_getControlEffort(int driveNumber);
		int32_t ll_getPositionDemandValue_cnt(int driveNumber);
		uint32_t ll_getDigitalInputs(int driveNumber);
		int16_t ll_getAnalogInput(int driveNumber);
		int32_t ll_getAuxilaryPositionActualValue(int driveNumber);
		int16_t ll_getCurrentActualValue(int driveNumber);
		
		
		private:
		ecmasterlib::EcMasterlibMain* etherCATStack;
		uint8_t* inBuffer;
		uint8_t* outBuffer;
		
		
		struct drive {
			int32_t posOffset		= 0;
			int32_t auxPosOffset	= 0;
			int32_t prevRawPos		= 0;
			int32_t prevRawAuxPos	= 0;
			int64_t absPos 			= 0;
			int64_t absAuxPos 		= 0;
			touchProbeStateEnum_ELMO touchProbeState = touchProbeStateEnum_ELMO::resetting;
			uint16_t touchProbeFunctionSet = 0x0;
// 			int32_t standardTouchProbeIndex	= 1;
		};
		
		drive drives[maxNumberOfDrives];
	};
}

#endif // ETHERCAT_INTERFACE_ELMO_HPP_
