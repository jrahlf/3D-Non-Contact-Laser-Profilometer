/*
 * utils.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: jonas
 */

#include "utils.h"
#include "project.h"
#include <xpcc/math/filter/pid.hpp>
#include <xpcc/architecture.hpp>
#include "control.h"
#include "encoder.h"

void Utils::enableSystick(){
	#if CONTROL_FREQUENCY == 1000
		xpcc::stm32::SysTickTimer::enable();
	#elif CONTROL_FREQUENCY == 10000
		xpcc::stm32::SysTickTimer::enable((F_CPU / 10000) - 1);
	#else
	#error bad control frequency
	#endif
}

void Utils::disableSystick(){
	xpcc::stm32::SysTickTimer::disable();
}

void Utils::startLoggingControl(){
	xpcc::Pid<float,1>::Parameter v_param = Control<Y>::getVPidParams();
	xpcc::Pid<float,1>::Parameter pos_param = Control<Y>::getPosPidParams();
	controlOut << "# Y-axis" << endl;
	controlOut << "#V_PID: "
			<< "  P: " << v_param.kp
			<< "  I: " << v_param.ki
			<< "  D: " << v_param.kd
			<< endl;
	controlOut << "#Pos_PID: "
			<< "  P: " << pos_param.kp
			<< "  I: " << pos_param.ki
			<< "  D: " << pos_param.kd
			<< endl;
	controlOut << "w_x x w_v v v_u v_ureal" << endl;
	Utils::enableSystick();
	xpcc::delay_ms(50);
}

template<Axis axis>
void driveToZero(){
	//sensors have pull ups
	if(HallSensor<axis>::getFrontState() == 0){
		Motor<axis>::setSpeed(0);
		return;
	}
	for(int i = 0; i < 100000; i++){
		int a = 0;
		while(HallSensor<axis>::getFrontState() == 1){
			Motor<axis>::setSpeed(-700);
			a++;
		}
		Motor<axis>::setSpeed(0);
	}
}

void Utils::calibrateZero(){
	driveToZero<X>();
	dout << "X axis calibrated" << endl;
	driveToZero<Y>();
	dout << "Y axis calibrated" << endl;
	xpcc::delay_ms(50); //let the mass come to rest
	Encoder::zero();
}
