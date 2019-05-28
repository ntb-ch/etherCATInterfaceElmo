#ifndef MOVINGCHAIR_CONTROLSYSTEM_SETELMOSEAT_HPP_
#define MOVINGCHAIR_CONTROLSYSTEM_SETELMOSEAT_HPP_

#include <eeros/control/Block.hpp>
#include <eeros/control/Input.hpp>
#include <eeros/control/Signal.hpp>

#include "MovingChairEtherCAT.hpp"

using namespace eeros::control;

namespace movingchair {
	
	class SetElmoSeat : public Block {
	
		typedef int32_t typeSetVelocity;
	
	public:
		SetElmoSeat(MovingChairEtherCAT& movingChairEtherCAT) :
			movingChairEtherCAT(movingChairEtherCAT)
		{ }
		
		virtual void run() {
			auto vel = velocity.getSignal().getValue();
			if (vel != vel ) {	// -> NaN
				vel = 0;
				std::cout << "ERROR, SetElmoSeat recives NaN" << std::endl;
			}
			movingChairEtherCAT.setVel(indexSeatDrive, vel);
		}
		
		
		virtual eeros::control::Input<typeSetVelocity>& getInVelocity() {
			return velocity;
		}
		
		protected:
			eeros::control::Input<typeSetVelocity> velocity;
		
			MovingChairEtherCAT& movingChairEtherCAT;
	};
};




#endif /* MOVINGCHAIR_CONTROLSYSTEM_SETELMOSEAT_HPP_ */