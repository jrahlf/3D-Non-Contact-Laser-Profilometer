/*
 * control.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: jonas
 */


#include "project.h"
#include "control.h"
#include "motor.h"
#include "hallSensor.h"
#include "securityController.h"

template<Axis axis>
class Motor;


template<Axis axis>
bool Control<axis>::init(){
	Control<axis>::v_is = 0;
	Control<axis>::v_shall = 0;
	Control<axis>::last_pos = 0;
	Control<axis>::positionControlEnabled = true;
	Control<axis>::speedControlEnabled = true;
	//param order: P, I, D, maxErrorSum, maxOutput
	Control<axis>::pos_pid = xpcc::Pid<float, 1>(CONTROLX_KP, CONTROLX_KI, CONTROLX_KD,
			CONTROLX_MAX_ERRORSUM, CONTROLX_MAX_OUTPUT);
	Control<axis>::v_pid = xpcc::Pid<float, 1>(CONTROLV_KP, CONTROLV_KI, CONTROLV_KD,
			CONTROLV_MAX_ERRORSUM, CONTROLV_MAX_OUTPUT);
	return true;
}


template<Axis axis>
void Control<axis>::update(bool externalLimitation){
	int enc = getIsPosition();

	if(positionControlEnabled){
		pos_pid.update(pos_shall - enc, externalLimitation);
		v_shall = pos_pid.getValue();
		/*if(abs(pos_shall - enc) <= CONTROLX_DEADBAND){
			v_shall = 0;
		}*/
	}

	v_is = (enc - last_pos)*CONTROL_FREQUENCY;
	last_pos = enc;

	if(speedControlEnabled){
		v_pid.update(v_shall - v_is);
		v_u = v_pid.getValue();
		v_ureal = v_u;
		SecurityController<axis>::saturate(v_ureal);
		if((abs(pos_shall - enc) <= CONTROLX_DEADBAND && positionControlEnabled)
				|| (!positionControlEnabled && abs(v_shall - v_is) < 1000)){
			v_ureal = 0;
			v_pid.reset();
			pos_pid.reset();
		}
		Motor<axis>::setSpeed(v_ureal);
	}
}

template<Axis axis>
void Control<axis>::setShallPosition(int position){
	Control<ALL>::resetPosPid();
	Control<ALL>::resetVPid();
	pos_shall = position;
}

//max speed is 5500rpm*400 counts/rev
template<Axis axis>
void Control<axis>::setShallSpeed(double speed){
	//disable position control
	Control<ALL>::enablePositionControl(false);
	if(speed > 1)
		speed = 1;
	if(speed < -1)
		speed = -1;
	v_shall = speed*MOTOR_MAX_SPEED;
}

template<Axis axis>
int Control<axis>::getSpeedErrorSum(){
	return v_pid.getErrorSum();
}

template<Axis axis>
int Control<axis>::getPositionErrorSum(){
	return pos_pid.getErrorSum();
}









