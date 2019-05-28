#ifndef BLOCK_TIMING_PERFORMANCE_TESTER_HPP
#define BLOCK_TIMING_PERFORMANCE_TESTER_HPP

#include <eeros/control/Block1i1o.hpp>
#include <eeros/control/Output.hpp>
#include <eeros/control/Signal.hpp>

using namespace eeros::control;
using namespace etherCATInterface;

template < typename T = double >
class block_timingPerformanceTester : public Block1i1o<T> {
public:
	block_timingPerformanceTester() : 
	enabled(false),
	counterTotal(0),
	counterHighLow(0),
	counterLowHigh(0),
	signalLow(0),
	signalHigh(10),
	inThreshold(5),
	lastCycleWasHigh(false)
	{
		this->out.getSignal().clear();
	}
	
	virtual void run() {
		if (enabled) {
// 			std::cout << "this->in.getSignal().getValue(): " << this->in.getSignal().getValue() << std::endl;
			if (this->in.getSignal().getValue() >= inThreshold ) {
				counterLowHigh++;
				if (lastCycleWasHigh) {		// signal stays high
					this->out.getSignal().setValue(signalLow);
				}
				else {	// signal switches to high
					std::cout << "counterLowHigh: " << counterLowHigh << std::endl;
					this->out.getSignal().setValue(signalLow);
					counterLowHigh = 0;
				}
// 				std::cout << "setValue(signalLow)" << std::endl;
				lastCycleWasHigh = true;
			}
			else {	// in signal is lower than inThreshold
				counterHighLow++;
				if (!lastCycleWasHigh) {		// signal stays low
					this->out.getSignal().setValue(signalHigh);
				}
				else {	// signal switches to low
					std::cout << "counterHighLow: " << counterHighLow << std::endl;
					this->out.getSignal().setValue(signalHigh);
					counterHighLow = 0;
				}
// 				std::cout << "setValue(signalHigh)" << std::endl;
				lastCycleWasHigh = false;
			}
			
// 			std::cout << std::endl;
			this->out.getSignal().setTimestamp(this->in.getSignal().getTimestamp());
			counterTotal++;
		}
	}
	
	void enable() { enabled = true; };
	void disable() { enabled = false; };
	
	virtual Output<T>& getOut() {
		return out;
	}
	
protected:
	Output<T> out;
	bool enabled;
	int counterTotal;
	int counterHighLow;
	int counterLowHigh;
	bool lastCycleWasHigh;
	T inThreshold;
	T signalHigh;
	T signalLow;
};


#endif /* BLOCK_TIMING_PERFORMANCE_TESTER_HPP */
