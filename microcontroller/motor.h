/*
 * motor.h
 *
 *  Created on: Nov 24, 2013
 *      Author: jonas
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "project.h"
#include "control.h"

template<Axis axis>
class Motor {

friend class Control<axis>;
friend class Motor<ALL>;

public:
	static bool init();

	struct Ext{
		/**
		 * Brakes the motor
		 */
		static void brake();

		/**
		 * Lets the motor run freely
		 */
		static void freeRun();
	};

friend class Motor<axis>::Ext;
friend class PC;

	static bool getFaultStatus();

public:
	//These functions shall be only called by a Controller

	/**
	 * Set the current speed
	 * @param speed in the range of [-1200,1200]
	 */
	static void setSpeed(int speed);

	/**
	 * Brakes the motor
	 */
	static void brake();

	/**
	 * Lets the motor run freely
	 */
	static void freeRun();




};


#endif /* MOTOR_H_ */
