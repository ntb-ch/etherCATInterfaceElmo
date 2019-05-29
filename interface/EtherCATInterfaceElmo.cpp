#include <EtherCATInterfaceElmo.hpp>

using namespace etherCATInterface;

EtherCATInterfaceElmo::EtherCATInterfaceElmo(ecmasterlib::EcMasterlibMain* etherCATStack, int numberOfDrives, int bytesPerSlaveTx, int bytesPerSlaveRx): 
numberOfDrives(numberOfDrives),
bytesPerSlaveTx(bytesPerSlaveTx),
bytesPerSlaveRx(bytesPerSlaveRx),
EtherCATInterfaceBase(etherCATStack, numberOfDrives, bytesPerSlaveTx, bytesPerSlaveRx)
{ }


//advanced set functions:
bool EtherCATInterfaceElmo::initAllDrives()
{
	return switchOnAllDrives();
}

bool EtherCATInterfaceElmo::recoverAllDrivesFromFault()
{
	return switchOnAllDrives();
}

bool EtherCATInterfaceElmo::switchOnAllDrives()
{
	bool allDrivesAreSwitchedOn = true;
	for (int driveNumber = 0; driveNumber < numberOfDrives; driveNumber++) {
		if (!switchOnDrive( driveNumber )) {
			allDrivesAreSwitchedOn = false;
		}
	}
	
	return allDrivesAreSwitchedOn;
}

bool EtherCATInterfaceElmo::switchOnDrive(int driveNumber)
{
	driveStatus_ELMO  driveState = getDriveStatusElmo(driveNumber);
	if ( driveState == driveStatus_ELMO::switchedOn)
        return true;

	// Elmo: GoldLine "CAN DS-402 Implementation Guide"		p.36
	// 0:	'start' -> 'not ready to switch on'					: The drive self-tests and/or self-initializes
	
	// 1:	'not ready to switch on' -> 'switch on disabled'	: The drive activates communication
	
	// 15: 	fault reset
// 	if (checkMaskedBits( driveState, faultValue, faultMask)) {
	if ( driveState == driveStatus_ELMO::fault ) {
		setControlWord(driveNumber, faultReset);
// 				std::cout << "drive " << driveNumber << " is in 15 fault reset" << std::endl;
	}
	
	
	// 2:	'switch on disabled' -> 'ready to switch on'		:
// 	if (checkMaskedBits( driveState, switchOnDisabledValue, switchOnDisabledMask)) {
	if ( driveState == driveStatus_ELMO::switchOnDisabled ) {
		setControlWord(driveNumber, shutdown);
// 				std::cout << "drive " << driveNumber << " is in 2: switch on disabled" << std::endl;
	}
	
	// 3:	'ready to switch on' -> 'switched on'				:
// 	if (checkMaskedBits( driveState, readyToSwitchOnValue, readyToSwitchOnMask)) {
	if ( driveState == driveStatus_ELMO::readyToSwitchOn ) {
		setControlWord(driveNumber, switchOn);	
// 				std::cout << "drive " << driveNumber << " is in 3: ready to switch on" << std::endl;
	}
	
	return false;
	
// 	if ( getDriveStatusElmo(int driveNumber) == driveStatus_ELMO::switchedOn)
//         return getIsDriveSwitchedOn( driveNumber ) ;
}

bool EtherCATInterfaceElmo::enableAllDrives()
{
	bool sucess = true;
	for ( int i; i < numberOfDrives; i++) {
		if( !enableDrive(i) ) sucess = false;
	}
	return sucess;
}

void EtherCATInterfaceElmo::disableAllDrives()
{
	for ( int i; i < numberOfDrives; i++) {
		disableDrive(i);
	}
}

void EtherCATInterfaceElmo::setControlWord(int driveNumber, controlWordCommand_ELMO word)
{
	switch(word) {
		case controlWordCommand_ELMO::shutdown :			ll_setControlWord(driveNumber, cwc_shutdown);
			break;
		case controlWordCommand_ELMO::switchOn :			ll_setControlWord(driveNumber, cwc_switchOn);
			break;
		case controlWordCommand_ELMO::switchOnAndEnable :	ll_setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::disableVoltage :		ll_setControlWord(driveNumber, cwc_switchOnAndEnable);
			break;
		case controlWordCommand_ELMO::quickStop :			ll_setControlWord(driveNumber, cwc_quickStop);
			break;
		case controlWordCommand_ELMO::disableOperation :	ll_setControlWord(driveNumber, cwc_disableOperation);
			break;
		case controlWordCommand_ELMO::enableOperation :		ll_setControlWord(driveNumber, cwc_enableOperation);
			break;
		case controlWordCommand_ELMO::faultReset :			ll_setControlWord(driveNumber, cwc_faultReset);
			break;
		default :
			break;
	}
}


//advanced get functions:
bool EtherCATInterfaceElmo::isAllDrivesReady()
{
	bool ready = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !getIsDriveReady(i) ) ready = false;
	}
	return ready;		
}

bool EtherCATInterfaceElmo::isAllDrivesEnabled()
{
	bool enabled = true;
	for ( int i; i < numberOfDrives; i++) {
		if ( !getIsDriveEnabled(i) ) enabled = false;
	}
	return enabled;
}

int64_t EtherCATInterfaceElmo::getPosition(int driveNumber)
{
	int32_t rawPos = ll_getPositionActualValue(driveNumber);
	int32_t diff = rawPos - drives[driveNumber].prevRawPos;
	drives[driveNumber].prevRawPos = rawPos;
	drives[driveNumber].absPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absPos + static_cast<int64_t>(drives[driveNumber].posOffset);
}

int64_t EtherCATInterfaceElmo::getPositionAux(int driveNumber)
{
	int32_t rawAuxPos = ll_getAuxilaryPositionActualValue(driveNumber);
	int32_t diff = rawAuxPos - drives[driveNumber].prevRawAuxPos;
	drives[driveNumber].prevRawAuxPos = rawAuxPos;
	drives[driveNumber].absAuxPos += static_cast<int64_t>(diff);
 	return drives[driveNumber].absAuxPos + static_cast<int64_t>(drives[driveNumber].auxPosOffset);
}

// gain scheduling functions (chair)
void EtherCATInterfaceElmo::disableVelocityControl(int driveNumber)
{
	ll_setGainSchedulingManualIndex(driveNumber, 2);
}

void EtherCATInterfaceElmo::enableVelocityControl(int driveNumber)
{
	ll_setGainSchedulingManualIndex(driveNumber, 1);
}


// basic functions
void EtherCATInterfaceElmo::disableDrive(int driveNumber)
{
	setControlWord(driveNumber, controlWordCommand_ELMO::disableOperation);
}

bool EtherCATInterfaceElmo::enableDrive(int driveNumber)
{
	if ( getIsDriveEnabled(driveNumber) ) return true;
	if ( !checkDriveStatus(driveNumber, driveStatus_ELMO::switchedOn) and getIsDriveEnabled(driveNumber) ) {
 		std::cout << "WARNING EtherCATInterfaceElmo::getDriveModeElmo():   EnableDrive(" << driveNumber << ") with status: 0x" << std::hex << ll_getStatusWord(driveNumber) << " not possible. It needs to be in state 'switched on'" << std::endl;
		return false;
	}
	else ll_setControlWord(driveNumber, cwc_enableOperation);
	return true;
}

void EtherCATInterfaceElmo::setModeOfOperation(int driveNumber, driveModeOfOperation_ELMO mode, bool scheduleGainIndex)
{
	int index;
	switch(mode) {
		case driveModeOfOperation_ELMO::CANEncoderMode :
			ll_setModeOfOperation(driveNumber, dmoov_CANEncoderMode);
			break;
		case driveModeOfOperation_ELMO::profilePosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profilePosition); 
			break;
		case driveModeOfOperation_ELMO::profileVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profileVelocity); 
			break;
		case driveModeOfOperation_ELMO::profileTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			ll_setModeOfOperation(driveNumber, dmoov_profileTorque); 
			break;
		case driveModeOfOperation_ELMO::homing :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_homing >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_homing);
			}
			ll_setModeOfOperation(driveNumber, dmoov_homing); 
			break;
		case driveModeOfOperation_ELMO::interpolatedPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_interpolatedPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousPosition :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_position >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_position);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousPosition); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousVelocity :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_velocity >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_velocity);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousVelocity); 
			break;
		case driveModeOfOperation_ELMO::cyclicSynchronousTorque :
			if ( scheduleGainIndex and (gainSchedulingManualIndex_torque >= 0) ) {
				ll_setGainSchedulingManualIndex(driveNumber, gainSchedulingManualIndex_torque);
			}
			ll_setModeOfOperation(driveNumber, dmoov_cyclicSynchronousTorque); 
			break;
		default : 
			break;
	}
}


bool EtherCATInterfaceElmo::getIsDriveEnabled(int driveNumber)
{
	return checkMaskedBits(ll_getStatusWord(driveNumber), operationEnabledValue, operationEnabledMask);
}

bool EtherCATInterfaceElmo::getIsDriveReady(int driveNumber)
{
	return getIsDriveSwitchedOn( driveNumber );
}

bool EtherCATInterfaceElmo::getIsDriveSwitchedOn(int driveNumber)
{
	return checkMaskedBits( ll_getStatusWord(driveNumber), switchedOnValue, switchedOnMask);
}

driveModeOfOperation_ELMO EtherCATInterfaceElmo::getDriveModeElmo(int driveNumber)
{
	int8_t modeOfOperation = ll_getModeOfOperationDisplay(driveNumber);
	switch( modeOfOperation ) {
		case dmoov_CANEncoderMode:				return driveModeOfOperation_ELMO::CANEncoderMode;
		case dmoov_profilePosition:				return driveModeOfOperation_ELMO::profilePosition;
		case dmoov_profileVelocity:				return driveModeOfOperation_ELMO::profileVelocity;
		case dmoov_profileTorque:				return driveModeOfOperation_ELMO::profileTorque;
		case dmoov_homing:						return driveModeOfOperation_ELMO::homing;
		case dmoov_interpolatedPosition:		return driveModeOfOperation_ELMO::interpolatedPosition;
		case dmoov_cyclicSynchronousPosition:	return driveModeOfOperation_ELMO::interpolatedPosition;
		case dmoov_cyclicSynchronousVelocity:	return driveModeOfOperation_ELMO::cyclicSynchronousVelocity;
		case dmoov_cyclicSynchronousTorque:		return driveModeOfOperation_ELMO::cyclicSynchronousTorque;
 		default:	std::string s;
 					std::cout << "ERROR EtherCATInterfaceElmo::getDriveModeElmo():   EtherCATInterfaceElmo: 0x" << std::hex << modeOfOperation  << std::dec << " is no valid mode of operation code" << std::endl;
	}
}

bool EtherCATInterfaceElmo::checkDriveStatus(int driveNumber, driveStatus_ELMO driveStatusToCheck)
{
	return getDriveStatusElmo(driveNumber) == driveStatusToCheck;
}

driveStatus_ELMO EtherCATInterfaceElmo::getDriveStatusElmo(int driveNumber)
{
	uint16_t statusWord = ll_getStatusWord(driveNumber);
	if ( checkMaskedBits(statusWord, notReadyToSwitchOnValue, notReadyToSwitchOnMask) ) {
		return driveStatus_ELMO::notReadyToSwitchOn;
	}
	if ( checkMaskedBits(statusWord, switchOnDisabledValue, switchOnDisabledMask) ) {
		return driveStatus_ELMO::switchOnDisabled;
	}
	if ( checkMaskedBits(statusWord, readyToSwitchOnValue, readyToSwitchOnMask) ) {
		return driveStatus_ELMO::readyToSwitchOn;
	}
	if ( checkMaskedBits(statusWord, switchedOnValue, switchedOnMask) ) {
		return driveStatus_ELMO::switchedOn;
	}
	if ( checkMaskedBits(statusWord, operationEnabledValue, operationEnabledMask) ) {
		return driveStatus_ELMO::operationEnabled;
	}
	if ( checkMaskedBits(statusWord, quickStopActiveValue, quickStopActiveMask) ) {
		return driveStatus_ELMO::quickStopActive;
	}
	if ( checkMaskedBits(statusWord, faultReactionActiveValue, faultReactionActiveMask) ) {
		return driveStatus_ELMO::faultReactionactive;
	}
	if ( checkMaskedBits(statusWord, faultValue, faultMask) ) {
		return driveStatus_ELMO::fault;
	}
	
	std::cout << "ERROR EtherCATInterfaceElmo::getDriveStatusElmo(" << driveNumber << "):   Methode returns: 0x" << std::hex << statusWord << std::dec << " which is not a valid status word" << std::endl;
}

std::__cxx11::string EtherCATInterfaceElmo::getDriveStatusStringElmo(int driveNumber)
{
	uint16_t statusWord = ll_getStatusWord(driveNumber);
	if ( checkMaskedBits(statusWord, notReadyToSwitchOnValue, notReadyToSwitchOnMask) ) {
		return "notReadyToSwitchOn";
	}
	if ( checkMaskedBits(statusWord, switchOnDisabledValue, switchOnDisabledMask) ) {
		return "switchOnDisabled";
	}
	if ( checkMaskedBits(statusWord, readyToSwitchOnValue, readyToSwitchOnMask) ) {
		return "readyToSwitchOn";
	}
	if ( checkMaskedBits(statusWord, switchedOnValue, switchedOnMask) ) {
		return "switchedOn";
	}
	if ( checkMaskedBits(statusWord, operationEnabledValue, operationEnabledMask) ) {
		return "operationEnabled";
	}
	if ( checkMaskedBits(statusWord, quickStopActiveValue, quickStopActiveMask) ) {
		return "quickStopActive";
	}
	if ( checkMaskedBits(statusWord, faultReactionActiveValue, faultReactionActiveMask) ) {
		return "faultReactionactive";
	}
	if ( checkMaskedBits(statusWord, faultValue, faultMask) ) {
		return "fault";
	}

	std::cout << "ERROR EtherCATInterfaceElmo::getDriveStatusStringElmo(" << driveNumber << "):   Methode returns: 0x" << std::hex << statusWord << std::dec << " which is not a valid status word" << std::endl;
}


// index pulse
bool EtherCATInterfaceElmo::enableCapturingIndexPulse(std::vector< int > driveNumbers )
{
	std::vector< int > touchProbes;
	touchProbes.resize( driveNumbers.size() );
	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
	return enableCapturingIndexPulse(driveNumbers, touchProbes);

}

bool EtherCATInterfaceElmo::enableCapturingIndexPulse(std::vector< int > driveNumbers, std::vector< int > touchProbes)
{
	if ( (driveNumbers.size()!=touchProbes.size()) ) {
		std::cout << "ERROR EtherCATInterfaceElmo::enableCapturingIndexPulse():   All argument vectors have to be of the same size" << std::endl;
		return false;
	}
	
	bool allProbesReady = true;
	for (	std::vector< int >::iterator	itDriveNumbers=driveNumbers.begin(),
											itTouchProbes=touchProbes.begin();
			itDriveNumbers != driveNumbers.end();
			++itDriveNumbers, ++itTouchProbes)
	{
			if ( !enableCapturingIndexPulse(*itDriveNumbers, *itTouchProbes) ) {
				allProbesReady = false;
			}
	}
	
	return allProbesReady;
}

bool EtherCATInterfaceElmo::enableCapturingIndexPulse(int driveNumber, int touchProbe)
{
	if ( touchProbe != 1 ) {
		std::cout << "ERROR EtherCATInterfaceElmo::enableCapturingIndexPulse():   touchProbe " << touchProbe << "is not implemented" << std::endl;
		return false;
	}
	
	switch(drives[driveNumber].touchProbeState) {
		case touchProbeStateEnum_ELMO::resetting :
			ll_setTouchProbeFunction( driveNumber, 0x0000 );		// disable everything
			drives[driveNumber].touchProbeFunctionSet = 0x0000;
			if ( ll_getTouchProbeStatus(driveNumber) == 0x0000 )	// both touch probes are switched off  -> stored values on elmo are reset
				drives[driveNumber].touchProbeState = touchProbeStateEnum_ELMO::enablingProbe;
			return false;
			break;
		case touchProbeStateEnum_ELMO::enablingProbe :
			ll_setTouchProbeFunction( driveNumber, 0x0001 );			//0x01 enable touchProbe1; disabled sampling -> stored value on elmo is reset
			drives[driveNumber].touchProbeFunctionSet = 0x0001;
			if ( ll_getTouchProbeStatus(driveNumber) == 0x0001 )	// touch probe 1 is enabled again
				drives[driveNumber].touchProbeState = touchProbeStateEnum_ELMO::enablingSampling;
			return false;
			break;
		case touchProbeStateEnum_ELMO::enablingSampling :
			ll_setTouchProbeFunction( driveNumber, 0x0031 );		//0x31 enable sampling on positive and negative edge
			drives[driveNumber].touchProbeFunctionSet = 0x0031;
			drives[driveNumber].touchProbeState = touchProbeStateEnum_ELMO::enabled;
			return true;
			break;
		case touchProbeStateEnum_ELMO::enabled :
			return true;
			break;
		default :
			std::cout << "ERROR EtherCATInterfaceElmo::enableCapturingIndexPulse():   touchProbeStateEnum does not exist" << std::endl;
	}
}

bool EtherCATInterfaceElmo::disableCapturingIndexPulse(int driveNumber, int touchProbe)
{
	if( touchProbe == 1 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::switchOffTouchProbe1);
	if( touchProbe == 2 ) setTouchProbeFunction(driveNumber, touchProbeFunctionEnum_ELMO::switchOffTouchProbe2);
}

void EtherCATInterfaceElmo::setOffsetAtIndexPos(std::vector< int > driveNumbers, std::vector< int > offsets, bool isAuxPos)
{
	std::vector< int > touchProbes;
	touchProbes.resize( driveNumbers.size() );
	std::fill( touchProbes.begin(), touchProbes.end(), 1 );		//allways use touchProbe 1
	return setOffsetAtIndexPos(driveNumbers, offsets, isAuxPos, touchProbes);

}

void EtherCATInterfaceElmo::setOffsetAtIndexPos(std::vector< int > driveNumbers, std::vector< int > offsets, bool isAuxPos, std::vector< int > touchProbes)
{
	for (	std::vector< int >::iterator itDriveNumbers=driveNumbers.begin(),
			itOffsets=offsets.begin(),
			itTouchProbes=touchProbes.begin();
			itDriveNumbers != driveNumbers.end();
			++itDriveNumbers, ++itOffsets, ++itTouchProbes) {
				setOffsetAtIndexPos(*itDriveNumbers, *itOffsets, isAuxPos, *itTouchProbes);
	}

}

void EtherCATInterfaceElmo::setOffsetAtIndexPos(int driveNumber, int offset, bool isAuxPos, int touchProbe)
{
	int32_t offs = static_cast<int32_t>( - static_cast<int>( getCapturedPosition(driveNumber, touchProbe) ) + offset );
	isAuxPos ? setPosAuxOffset(driveNumber, offs) : setPosOffset(driveNumber, offs);
}

void EtherCATInterfaceElmo::setPosOffset(int driveNumber, int32_t offset)
{
	if ( driveNumber < numberOfDrives )	{
		drives[driveNumber].posOffset = offset;
	}
		else {
		std::cout << "ERROR EtherCATInterfaceElmo::setPosOffset():   driveNumber '" << driveNumber << "' is not valid." << std::endl;
	}
}

void EtherCATInterfaceElmo::setPosAuxOffset(int driveNumber, int32_t offset)
{
	if ( driveNumber < numberOfDrives )	{
		drives[driveNumber].auxPosOffset = offset;
	}
		else {
		std::cout << "ERROR EtherCATInterfaceElmo::setPosAuxOffset():   driveNumber '" << driveNumber << "' is not valid." << std::endl;
	}
}

void EtherCATInterfaceElmo::setTouchProbeFunction(int driveNumber, touchProbeFunctionEnum_ELMO function)
{
	uint16_t valueToSet = drives[driveNumber].touchProbeFunctionSet;
// 	uint16_t valueToSet = 0x0;
	switch ( function ) {
		case touchProbeFunctionEnum_ELMO::switchOffTouchProbe1:
			valueToSet &= ~0x1;
			break;
		case touchProbeFunctionEnum_ELMO::enableTouchProbe1:
			valueToSet |= 0x1;
			break;
		case touchProbeFunctionEnum_ELMO::triggerWithTouchProbe1Input:
			valueToSet &= ~(0x1 << 2);
			valueToSet &= ~(0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWitchZeroImpulsSignalOrPositioinEncoder1:
			valueToSet |=  (0x1 << 2);
			valueToSet &= ~(0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::touchProbeSourceAsDefinedInObject0x60D0SI01:
			valueToSet &= ~(0x1 << 2);
			valueToSet |=  (0x1 << 3);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtThePositiveEdgeOfTouchProbe1:
			valueToSet &= ~(0x1 << 4);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1:
			valueToSet |=  (0x1 << 4);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtTheNegativeEdgeOfTouchProbe1:
			valueToSet &= ~(0x1 << 5);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtTheNegativeEdgeOfTouchProbe1:
			valueToSet |=  (0x1 << 5);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffTouchProbe2:
			valueToSet &= ~(0x1 << 8);
			break;
		case touchProbeFunctionEnum_ELMO::enableTouchProbe2:
			valueToSet |=  (0x1 << 8);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWithTouchProbe2Input:
			valueToSet &= ~(0x1 << 10);
			valueToSet &= ~(0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::triggerWitchZeroImpulsSignalOrPositioinEncoder2:
			valueToSet |=  (0x1 << 10);
			valueToSet &= ~(0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::touchProbeSourceAsDefinedInObject0x60D0SI02:
			valueToSet &= ~(0x1 << 10);
			valueToSet |=  (0x1 << 11);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtThePositiveEdgeOfTouchProbe2:
			valueToSet &= ~(0x1 << 12);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe2:
			valueToSet |=  (0x1 << 12);
			break;
		case touchProbeFunctionEnum_ELMO::switchOffSamplingAtTheNegativeEdgeOfTouchProbe2:
			valueToSet &= ~(0x1 << 13);
			break;
		case touchProbeFunctionEnum_ELMO::enableSamplingAtTheNegativeEdgeOfTouchProbe2:
			valueToSet |=  (0x1 << 13);
			break;
	}
	
	drives[driveNumber].touchProbeFunctionSet = valueToSet;
	ll_setTouchProbeFunction(driveNumber, valueToSet);
}



bool EtherCATInterfaceElmo::getTouchProbeIsEnabled(int driveNumber, int touchProbe)
{
// 	std::cout << "ELMO: getTouchProbeIsEnabled: driveNumber: " << driveNumber << "   touchProbe: " << touchProbe << std::endl;
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1EnabledValue, touchProbe1EnabledMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1EnabledValue, touchProbe1EnabledMask) ) return true;
	}
	else	std::cout << "ERROR EtherCATInterfaceElmo::getTouchProbeIsEnabled():   TouchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulseIsCaptured(int driveNumber, int touchProbe)
{
	if ( getIndexPulsePositiveEdgeIsCaptured(driveNumber, touchProbe) || getIndexPulseNegativeEdgeIsCaptured(driveNumber, touchProbe) ) {
		return true;
	}
	
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulsePositiveEdgeIsCaptured(int driveNumber, int touchProbe)
{
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1PositiveEdgeStoredValue, touchProbe1PositiveEdgeStoredMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe2PositiveEdgeStoredValue, touchProbe2PositiveEdgeStoredMask) ) return true;
	}
	else	std::cout << "ERROR: EtherCATInterfaceElmo::getIndexPulsePositiveEdgeIsCaptured():   TouchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

bool EtherCATInterfaceElmo::getIndexPulseNegativeEdgeIsCaptured(int driveNumber, int touchProbe)
{
	uint16_t touchProbeStatus = ll_getTouchProbeStatus(driveNumber);
	if	( touchProbe == 1 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe1NegativeEdgeStoredValue, touchProbe1NegativeEdgeStoredMask) ) return true;
	}
	else if	( touchProbe == 2 ) {
		if ( checkMaskedBits(touchProbeStatus, touchProbe2NegativeEdgeStoredValue, touchProbe2NegativeEdgeStoredMask) ) return true;
	}
	else	std::cout << "ERROR: EtherCATInterfaceElmo::getIndexPulseNegativeEdgeIsCaptured():   TouchProbe " << touchProbe << " does not exists." << std::endl;
	return false;
}

int32_t EtherCATInterfaceElmo::getCapturedPosition(int driveNumber, int touchProbe)	// compensated for direction of rotation
{
	return getCapturedPositionPositivePulse(driveNumber, touchProbe);
}

int32_t EtherCATInterfaceElmo::getCapturedPositionPositivePulse(int driveNumber, int touchProbe)
{
	if	( touchProbe == 1 )		return ll_getTouchProbePos1Positive(driveNumber);
	else if	( touchProbe == 2 )	return ll_getTouchProbePos2Positive(driveNumber);
	else						std::cout << "ERROR EtherCATInterfaceElmo::getCapturedPosition():   TouchProbe " << touchProbe << " does not exists." << std::endl;
	return 0;
}

int32_t EtherCATInterfaceElmo::getCapturedPositionNegativePulse(int driveNumber, int touchProbe)
{
	if	( touchProbe == 1 )		return ll_getTouchProbePos1Negative(driveNumber);
	else if	( touchProbe == 2 )	std::cout << "No TouchProbePosition negative pulse for TouchProbe 2." << std::endl;
	else						std::cout << "ERROR EtherCATInterfaceElmo::getCapturedPositionNegativePulse():   TouchProbe " << touchProbe << " does not exists." << std::endl;
	return 0;
}


// Basic get and set functions to get/write values from/to buffer array
// ////////////////////////////////////////////////////////////////////

//basic set functions:
void EtherCATInterfaceElmo::ll_setControlWord(int driveNumber, uint16_t controlWord)
{
	set16bit(oo_controlWord, driveNumber, controlWord);
}

void EtherCATInterfaceElmo::ll_setModeOfOperation(int driveNumber, int8_t modeOfOperation)
{
	set8bit(oo_modeOfOperation, driveNumber, modeOfOperation);
}

void EtherCATInterfaceElmo::ll_setTargetTorque(int driveNumber, int16_t targetTorque)
{
	set16bit(oo_targetTorque, driveNumber, targetTorque);
}

void EtherCATInterfaceElmo::ll_setMaxTorque(int driveNumber, int16_t maxTorque)
{
	set16bit(oo_maxTorque, driveNumber, maxTorque);
}

void EtherCATInterfaceElmo::ll_setMaxCurrent(int driveNumber, int16_t maxCurrent)
{
	set16bit(oo_maxCurrent, driveNumber, maxCurrent);
}

void EtherCATInterfaceElmo::ll_setTargetPosition(int driveNumber, int32_t targetPosition)
{
	set32bit(oo_targetPosition, driveNumber, targetPosition);
}

void EtherCATInterfaceElmo::ll_setMaxProfileVelocity(int driveNumber, uint32_t maxProfileVelocity)
{
	set32bit(oo_maxProfileVelocity, driveNumber, maxProfileVelocity);
}

void EtherCATInterfaceElmo::ll_setProfileVelocity(int driveNumber, uint32_t profileVelocity)
{
	set32bit(oo_profileVelocity, driveNumber, profileVelocity);
}

void EtherCATInterfaceElmo::ll_setEndVelocity(int driveNumber, uint32_t endVelocity)
{
	set32bit(oo_endVelocity, driveNumber, endVelocity);
}

void EtherCATInterfaceElmo::ll_setProfileAcceleration(int driveNumber, uint32_t profileAcceleration)
{
	set32bit(oo_profileAcceleration, driveNumber, profileAcceleration);
}

void EtherCATInterfaceElmo::ll_setProfileDeceleration(int driveNumber, uint32_t profileDeceleration)
{
	set32bit(oo_profileDeceleration, driveNumber, profileDeceleration);
}

void EtherCATInterfaceElmo::ll_setTorqueSlope(int driveNumber, uint32_t torqueSlope)
{
	set32bit(oo_torqueSlope, driveNumber, torqueSlope);
}

void EtherCATInterfaceElmo::ll_setPositionOffset(int driveNumber, int32_t positionOffset)
{
	set32bit(oo_positionOffset, driveNumber, positionOffset);
}

void EtherCATInterfaceElmo::ll_setVelocityOffset(int driveNumber, int32_t velocityOffset)
{
	set32bit(oo_velocityOffset, driveNumber, velocityOffset);
}

void EtherCATInterfaceElmo::ll_setTorqueOffset(int driveNumber, int16_t torqueOffset)
{
	set16bit(oo_torqueOffset, driveNumber, torqueOffset);
}

void EtherCATInterfaceElmo::ll_setTouchProbeFunction(int driveNumber, uint16_t touchProbeFunction)
{
	set16bit(oo_touchProbeFunction, driveNumber, touchProbeFunction);
}

void EtherCATInterfaceElmo::ll_setInterpolatedDataRecord1(int driveNumber, int32_t interpolatedDataRecord1)
{
	set32bit(oo_interpolatedDataRecord_1, driveNumber, interpolatedDataRecord1);
}

void EtherCATInterfaceElmo::ll_setInterpolatedDataRecord2(int driveNumber, int32_t interpolatedDataRecord2)
{
	set32bit(oo_interpolatedDataRecord_2, driveNumber, interpolatedDataRecord2);
}

void EtherCATInterfaceElmo::ll_setTargetVelocity(int driveNumber, int32_t targetVelocity)
{
    std::cout << "ll_setTargetVelocity   drive: " << driveNumber << "   targetVel: " << targetVelocity << std::endl;
	set32bit(oo_targetVelocity, driveNumber, targetVelocity);
}

void EtherCATInterfaceElmo::ll_setDigitalOutput(int driveNumber, uint32_t digitalOutput)
{
	set32bit(oo_digitalOutput, driveNumber, digitalOutput);
}

void EtherCATInterfaceElmo::ll_setPolarity(int driveNumber, uint8_t polarity)
{
	set8bit(oo_polarity, driveNumber, polarity);
}

void EtherCATInterfaceElmo::ll_setGainSchedulingManualIndex(int driveNumber, uint16_t index)
{
	set16bit(oo_gainSchedlingManualIndex, driveNumber, index);
}


//basic get functions:
uint16_t EtherCATInterfaceElmo::ll_getStatusWord(int driveNumber)
{
	return (uint16_t)get16bit(io_statusWord, driveNumber);
}

int8_t EtherCATInterfaceElmo::ll_getModeOfOperationDisplay(int driveNumber)
{
	return (int8_t)get8bit(io_modeOfOperationDisplay, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionDemand_UU(int driveNumber)
{
	return (int32_t)get32bit(io_postionDemand_UU, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getActualPosition_counts(int driveNumber)
{
	return (int32_t)get32bit(io_actualPosition_counts, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_positionActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocitySensorActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_velocitySensorActualValue, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocityDemand(int driveNumber)
{
	return (int32_t)get32bit(io_velocityDemand, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getVelocityActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_velocityActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getTorqueDemandValue(int driveNumber)
{
	return (int16_t)get16bit(io_torqueDemandValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getTorqueActualValue(int driveNumber)
{
	return (int16_t)get16bit(io_torqueActualValue, driveNumber);
}

uint16_t EtherCATInterfaceElmo::ll_getTouchProbeStatus(int driveNumber)
{
	return (uint16_t)get16bit(io_touchProbeStatus, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos1Positive(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos1Positive, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos1Negative(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos1Negative, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getTouchProbePos2Positive(int driveNumber)
{
	return (int32_t)get32bit(io_touchProbePos2Positive, driveNumber);
}

uint32_t EtherCATInterfaceElmo::ll_getDCLinkCircuitVoltage(int driveNumber)
{
	return (uint32_t)get32bit(io_DCLinkCircuitVoltage, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionFollowingError(int driveNumber)
{
	return (int32_t)get32bit(io_positionFollowingError, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getControlEffort(int driveNumber)
{
	return (int32_t)get32bit(io_controlEffort, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getPositionDemandValue_cnt(int driveNumber)
{
	return (int32_t)get32bit(io_positionDemandValue_cnt, driveNumber);
}

uint32_t EtherCATInterfaceElmo::ll_getDigitalInputs(int driveNumber)
{
	return (uint32_t)get32bit(io_digitalInputs, driveNumber);;
}

int16_t EtherCATInterfaceElmo::ll_getAnalogInput(int driveNumber)
{
	return (int16_t)get16bit(io_analogInput, driveNumber);
}

int32_t EtherCATInterfaceElmo::ll_getAuxilaryPositionActualValue(int driveNumber)
{
	return (int32_t)get32bit(io_auxilaryPositionActualValue, driveNumber);
}

int16_t EtherCATInterfaceElmo::ll_getCurrentActualValue(int driveNumber)
{
	return (int16_t)get16bit(io_currentActualValue, driveNumber);
}
