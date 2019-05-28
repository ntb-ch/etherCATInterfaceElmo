#ifndef BLOCK_PRINT_NUMBER_HPP_
#define BLOCK_PRINT_NUMBER_HPP_

#include <eeros/control/Block1i.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <type_traits>


using namespace eeros::logger;
// namespace eeros {

template <typename Tnumber = double>
class block_printNumber : public eeros::control::Block1i<Tnumber> {

public:
// 	block_printNumber(Logger& log) : block_printNumber(log, "", "") {};
	
// 	block_printNumber(Logger& log, std::string prefix) : block_printNumber(log, prefix, "") {}
		
	block_printNumber(Logger& log, std::string prefix="", std::string suffix="", int divider=0, bool enabled=true) : 
		log(log),
		prefix(prefix),
		suffix(suffix),
		divider(divider),
		enabled(enabled),
		counter(1)
	{}

	virtual void run() {
		if ( enabled ) {
			if ( divider==0 or counter%divider==0 ) {
				log.info() << prefix  << this->in.getSignal().getValue() << suffix;
	// 			this->get
				counter = 0;
			}
			counter++;
		}
	}
	
	void enable() { enabled = true; };
	void disable() { enabled = false; };


protected:


private:
	std::string prefix;
	std::string suffix;
	Logger& log;
	int divider;
	int counter;
	
	bool enabled;
// 	template <typename S>
// 	typename std::enable_if<!elementWise,S>::type calculateResults(S value) {
// 		return gain * value;
// 	}
// 
// 
// 	template <typename S>
// 	typename std::enable_if<elementWise,S>::type calculateResults(S value) {
// 		return value.multiplyElementWise(gain);
// 	}
// 
// 
// 	template <typename S>
// 	typename std::enable_if<std::is_integral<S>::value>::type resetMinMaxGain() {
// 		minGain = std::numeric_limits<int32_t>::min();
// 		maxGain = std::numeric_limits<int32_t>::max();
// 	}
// 
// 
// 	template <typename S>
// 	typename std::enable_if<std::is_floating_point<S>::value>::type resetMinMaxGain() {
// 		minGain = std::numeric_limits<double>::lowest();
// 		maxGain = std::numeric_limits<double>::max();
// 	}
// 
// 
// 	template <typename S>
// 	typename std::enable_if<!std::is_arithmetic<S>::value && std::is_integral<typename S::value_type>::value>::type resetMinMaxGain() {
// 		minGain.fill(std::numeric_limits<int32_t>::min());
// 		maxGain.fill(std::numeric_limits<int32_t>::max());
// 	}
// 
// 
// 	template <typename S>
// 	typename std::enable_if<!std::is_arithmetic<S>::value && std::is_floating_point<typename S::value_type>::value>::type resetMinMaxGain() {
// 		minGain.fill(std::numeric_limits<double>::lowest());
// 		maxGain.fill(std::numeric_limits<double>::max());
// 	}
};

// /*
// /**
// 	* Operator overload (<<) to enable an easy way to print the state of a
// 	* Gain instance to an output stream.\n
// 	* Does not print a newline control character.
// 	*/
// template <typename Tout, typename Tgain>
// std::ostream& operator<<(std::ostream& os, Gain<Tout,Tgain>& gain) {
// 	os << "Block Gain: '" << gain.getName() << "' is enabled=" << gain.enabled << ", gain=" << gain.gain << ", ";
// 	os << "smoothChange=" << gain.smoothChange << ", minGain=" << gain.minGain << ", maxGain=" << gain.maxGain << ", ";
// 	os << "targetGain=" << gain.targetGain << ", gainDiff=" << gain.gainDiff;
// }*/

// };

#endif /* BLOCK_PRINT_NUMBER_HPP_ */
