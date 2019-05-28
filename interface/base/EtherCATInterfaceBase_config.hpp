#ifndef ETHERCAT_INTERFACE_BASE_CONFIG_HPP_
#define ETHERCAT_INTERFACE_BASE_CONFIG_HPP_


namespace etherCATInterface {
	

	// Configurations for network
	// ///////////////////////////
	static constexpr int numberOfDrives = 2;
	static constexpr int bytesPerPDOFrameTx = 32;	// eni.xml --> <EtherCATConfig> <Config> <ProcessImage> <Outputs> <ByteSize>
	static constexpr int bytesPerPDOFrameRx = 32;	// eni.xml --> <EtherCATConfig> <Config> <ProcessImage> <Inputs> <ByteSize>
	static constexpr int bytesPerSlaveTx = bytesPerPDOFrameTx / numberOfDrives;
	static constexpr int bytesPerSlaveRx = bytesPerPDOFrameRx / numberOfDrives;
	


}

#endif // ETHERCAT_INTERFACE_BASE_CONFIG_HPP_