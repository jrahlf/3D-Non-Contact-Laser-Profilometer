/*
 * transform.h
 *
 *  Created on: Jan 23, 2014
 *      Author: jonas
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "project.h"

struct Transform{
	static int mmToEncoder(float mm){
		return int(MOTOR_ENC_PER_REV*MOTOR_GEAR_RATIO*mm + 0.5f);
	}
	static float encoderToMM(int encoder){
		return float(encoder)/float(MOTOR_ENC_PER_REV*MOTOR_GEAR_RATIO);
	}
	static int mumToEncoder(float mum){
		return int(mum*2 + 0.5f);
	}
	static float encoderToMUM(int encoder){
		return float(encoder)*0.5f;
	}
};



#endif /* TRANSFORM_H_ */
