/*
 * securityController.h
 *
 *  Created on: Dec 28, 2013
 *      Author: jonas
 */

#ifndef SECURITYCONTROLLER_H_
#define SECURITYCONTROLLER_H_

#include "project.h"
#include "hallSensor.h"
#include "state.h"

template <Axis axis> class HallSensor;

template<Axis axis>
class SecurityController{
public:

	template<typename T>
	static void saturate(T& value){
		if((!HallSensor<axis>::getEndState()) && value > 0){
			value = 0;
			//bool clear = HallSensor<axis>::checkEndState();
			//State::setSecondaryState(State::MOTOR_LIMITED, !clear);
		}else if((!HallSensor<axis>::getFrontState()) && value < 0){
			value = 0;
			//bool clear = HallSensor<axis>::checkFrontState();
			//State::setSecondaryState(State::MOTOR_LIMITED, !clear);
		}
	}
};


#endif /* SECURITYCONTROLLER_H_ */
