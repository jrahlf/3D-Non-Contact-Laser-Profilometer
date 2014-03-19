/*
 * control.cpp
 *
 *  Created on: Dec 29, 2013
 *      Author: jonas
 */

#include "control.h"

template<> float Control<ALL>::maxSpeed = 1;

template<>
bool Control<ALL>::init(){
	return Control<X>::init() && Control<Y>::init();
}

template<>
void Control<ALL>::update(bool externalLimitation){
	Control<X>::update(externalLimitation);
	Control<Y>::update(externalLimitation);
}

template<>
int32_t Control<Y>::getIsPosition(){
	return int32_t(xpcc::stm32::Timer2::getValue());
}

template<>
int32_t Control<X>::getIsPosition(){
	return int32_t(xpcc::stm32::Timer5::getValue());
}

template<>
void Control<ALL>::resetVPid(){
	Control<X>::v_pid.reset();
	Control<Y>::v_pid.reset();
}

template<>
void Control<ALL>::resetPosPid(){
	Control<X>::pos_pid.reset();
	Control<Y>::pos_pid.reset();
}

template<>
void Control<ALL>::enableSpeedControl(bool enable){
	Control<ALL>::resetVPid();
	Control<X>::speedControlEnabled = enable;
	Control<Y>::speedControlEnabled = enable;
}

template<>
void Control<ALL>::enablePositionControl(bool enable){
	if(enable){
		Control<ALL>::enableSpeedControl(true);
		Control<X>::setShallPosition(Control<X>::getIsPosition());
		Control<Y>::setShallPosition(Control<Y>::getIsPosition());
	}
	Control<X>::positionControlEnabled = enable;
	Control<Y>::positionControlEnabled = enable;
	Control<ALL>::resetPosPid();
}

template<>
void Control<ALL>::setMaxSpeed(float maxSpeed){
	if(maxSpeed > 1)
		maxSpeed = 1;
	if(maxSpeed < 0)
		maxSpeed = 0;
	int speed = MOTOR_MAX_SPEED*maxSpeed;
	Control<X>::pos_pid.setMaxOutput(speed);
	Control<Y>::pos_pid.setMaxOutput(speed);
	Control<ALL>::maxSpeed = maxSpeed;
}


