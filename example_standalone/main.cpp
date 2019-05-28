#include <iostream>
#include <signal.h>

#include <EcMasterlibMain.hpp>

#include <EtherCATInterfaceElmo.hpp>
using namespace etherCATInterface;




ecmasterlib::EcMasterlibMain* etherCATStackPtr;
EtherCATInterfaceElmo* elmoDrivesPtr;

bool running = true;


static constexpr int byteSizePerSlave = 32;
static constexpr int numberOfDrivesTotal = 2;



static void signalHandler( int nSignal )
{
    std::cout << "signalHandler" << std::endl;
	if (elmoDrivesPtr) elmoDrivesPtr->disableAllDrives();
    if (etherCATStackPtr) etherCATStackPtr->stop();
    running = false;
    std::cout << "signalHandler" << std::endl;
}
// 

int main(int argc, char **argv) {
	signal(SIGINT, signalHandler);



	// EtherCAT
	// ////////////////////////////////////////////////////////////////////////
	auto etherCATStack = ecmasterlib::EcMasterlibMain::createInstance(argc, argv, byteSizePerSlave*numberOfDrivesTotal);
	signal(SIGINT, signalHandler);
	etherCATStackPtr = etherCATStack;
	sleep(9);
	EtherCATInterfaceElmo elmoDrives = EtherCATInterfaceElmo( etherCATStack );
	elmoDrivesPtr = &elmoDrives;
	//   movingchair::MovingChairEtherCAT chairEtherCAT(etherCATStack);
	//	while(1) { sleep(1); }
	//   chairEtherCAT.initElmoDrives();
	//   signal(SIGINT, signalHandler);  

	
	
	auto cv = etherCATStack->getConditionalVariable();
	auto m = etherCATStack->getMutex();
	auto cv2 = etherCATStack->getConditionalVariable2();
	auto m2 = etherCATStack->getMutex2();


	while ( !elmoDrives.initAllDrives() and running ) { 
		sleep(1); 
		std::cout << "MAIN: initAllDrives" << std::endl;
// 		cv->notify_one();
	};


		std::cout << "MAIN: enableDrive(0)" << std::endl;
		elmoDrives.enableDrive(0);

// 	while ( running ) {
// 		std::cout << "loop" << std::endl;
// 		sleep(1);
// 	};
	
// 	elmoDrives.initAllDrives();
	
	int count = 0;
		elmoDrives.ll_setDigitalOutput(0, 0xFFFFFFFF);
		elmoDrives.ll_setDigitalOutput(1, 0xFFFFFFFF);
// 		elmoDrives.setDigitalOutput(0, 0);
// 		elmoDrives.setDigitalOutput(1, 0);
	
	int delay = 0;
	
// 	etherCATStack->wait = true;
	
	
	std::cout << "setting mode of operation: 0" << std::endl;
	elmoDrives.setModeOfOperation(0, etherCATInterface::cyclicSynchronousVelocity);
	sleep(0.1);
	std::cout << "elmoDrives.ll_setTargetVelocity(0, 5000), Status:" << elmoDrives.getDriveStatusStringElmo(0) << std::endl;
	elmoDrives.ll_setTargetVelocity(0, 5000);
	sleep(5);
	std::cout << "elmoDrives.ll_setTargetVelocity(0, 0), Status: " << elmoDrives.getDriveStatusStringElmo(0) << std::endl;
	elmoDrives.ll_setTargetVelocity(0, 0);
	sleep(5);
		
	while (running) {
		// Synchronisation with ecmasterlib
		// ////////////////////////////////////////////////////////////////////
		std::unique_lock<std::mutex> lk(*m);
		cv->wait(lk);
		lk.unlock();
		// ////////////////////////////////////////////////////////////////////
		

		
		// ////////////////////////////////////////////////////////////////////
		if ( count%500 == 0 ) {
			std::cout << "delay: " << delay << ", count: " << count << std::endl;
			delay = 0;
			if ( elmoDrives.ll_getDigitalInputs(1) == 0x3f0000 ) {
				elmoDrives.ll_setDigitalOutput(0, 0xFFFFFFFF);
			}
			else {
				elmoDrives.ll_setDigitalOutput(0, 0x0);
			}
			std::cout << "loop DI1: " << std::hex << elmoDrives.ll_getDigitalInputs(1) << std::endl;
		}
		if ( elmoDrives.ll_getDigitalInputs(1) == 0x3f0000 ) {
			delay++;
		}
		
		
// 		if (count == 3000 ) {
// 			std::cout << "enableTouchProbe1" << std::endl;
// // 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableTouchProbe1);
// 			elmoDrives.ll_setTouchProbeFunction(0, 0x31);
// 		}
// 		if (count == 4000) {
// // 			std::cout << "enabling sampling" << std::endl;
// // 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1);
// 		}
// 		
// // 		elmoDrives.getTouchProbeIsEnabled(1);
// 		if (!elmoDrives.getTouchProbeIsEnabled(0)) {
// // 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableTouchProbe1);
// // 			elmoDrives.setTouchProbeFunction(0, touchProbeFunctionEnum_ELMO::enableSamplingAtThePositiveEdgeOfTouchProbe1);
// // 			std::cout << "enabeling enableSamplingAtThePositiveEdgeOfTouchProbe1" << std::endl;
// 		}
// 		else {
// // 			std::cout << "index enabled" << std::endl;
// 			if ( elmoDrives.getIndexPulseIsCaptured(0) and count%500 == 0) {
// 				std::cout << "index positive captured at: " << elmoDrives.getCapturedPositionPositivePulse(0);
// 			}
// // 		}
// 		
// 		
// 		if ( count%1000 == 0 ) {
// 			std::cout << "MAIN: getTouchProbeStatus(0): 0x" << std::hex << elmoDrives.ll_getTouchProbeStatus(0) << std::endl;
// 			std::cout << "MAIN: getTouchProbePosition(0): 0x" << std::hex << elmoDrives.ll_getTouchProbePos1Positive(0) << std::endl;
// 			std::cout << "                               position: " << elmoDrives.ll_getPositionActualValue(0);
// 		}
		
		
		count++;
// 		cv2->notify_one();
	}

	//   sleep(5);
	//   





	return 0;
}
