/*
 * control.h
 *
 *  Created on: Nov 24, 2013
 *      Author: jonas
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "project.h"
#include <xpcc/math/filter/pid.hpp>


template<Axis axis>
class Control{

	friend class Control<ALL>;

private:
	static volatile int pos_shall;
	static volatile int v_is; // velocity in pulses/s
	static volatile int v_shall;

	static float maxSpeed;
	static xpcc::Pid<float, 1> v_pid;
	static xpcc::Pid<float, 1> pos_pid;
	static int v_u; // speed controller output
	static int v_ureal;
	static int last_pos; //position when the last control loop was executed

	static bool positionControlEnabled;
	static bool speedControlEnabled;

public:

	static bool init();
	static void enablePositionControl(bool enable);
	static void enableSpeedControl(bool enable);
	static void setShallPosition(int x);
	/**
	 * Also disables the position controller
	 * @param speed [-1,1]
	 */
	static void setShallSpeed(double speed);
	static int getSpeedErrorSum();
	static int getPositionErrorSum();
	static int32_t getIsPosition();
	static inline int getShallSpeed(){ return v_shall; }
	static inline int getIsSpeed(){ return v_is; }
	static inline int getShallPosition(){ return pos_shall; }
	static inline int getSpeedU(){ return v_u; }
	static inline int getSpeedRealU(){ return v_ureal; }
	static inline int getPosU(){ return v_shall; } //only valid if position controller is active
	static const xpcc::Pid<float,1>::Parameter& getVPidParams(){ return v_pid.getParameter(); }
	static const xpcc::Pid<float,1>::Parameter& getPosPidParams(){ return pos_pid.getParameter(); }
	static bool isPositionControlEnabled(){ return positionControlEnabled; }
	static bool isSpeedControlEnabled(){ return speedControlEnabled; }
	static void resetVPid();
	static void resetPosPid();
	static float getMaxSpeed(){ return maxSpeed; }
	/**
	 * [0, 1]
	 * Limits the output of the position controller
	 */
	static void setMaxSpeed(float maxSpeed);

	/**
	 * To be called periodically (1kHz)
	 * Computes new ouput values for the H-bridge
	 */
	static void update(bool externalLimitation);

};

template<Axis axis> volatile int Control<axis>::pos_shall;
template<Axis axis> volatile int Control<axis>::v_is;
template<Axis axis> volatile int Control<axis>::v_shall;
template<Axis axis> int Control<axis>::v_u;
template<Axis axis> int Control<axis>::v_ureal;
template<Axis axis> float Control<axis>::maxSpeed;

template<Axis axis> bool Control<axis>::positionControlEnabled;
template<Axis axis> bool Control<axis>::speedControlEnabled;

template<Axis axis> xpcc::Pid<float, 1> Control<axis>::v_pid;
template<Axis axis> xpcc::Pid<float, 1> Control<axis>::pos_pid;
template<Axis axis> int Control<axis>::last_pos; //position when the last control loop was executed

template<> void Control<ALL>::update(bool externalLimitation);
template<> int32_t Control<X>::getIsPosition();
template<> int32_t Control<Y>::getIsPosition();
template<> bool Control<ALL>::init();

#include "control_impl.h"


#endif /* CONTROL_H_ */
