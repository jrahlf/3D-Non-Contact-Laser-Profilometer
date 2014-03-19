/*
 * hallSensor.h
 *
 *  Created on: Dec 23, 2013
 *      Author: jonas
 */

#ifndef HALLSENSOR_H_
#define HALLSENSOR_H_

//#pragma GCC push_options
//#pragma GCC optimize ("O0")

#include "project.h"
#include "control.h"

template<Axis axis>
class HallSensor{
private:
	static volatile bool triggered[2];
	static volatile bool state[2]; //the real current state
	static volatile int triggeredPosition[2];

	GPIO__INPUT(XFRONT, E, 11);
	GPIO__INPUT(XEND, E, 7);
	GPIO__INPUT(YFRONT, D, 8);
	GPIO__INPUT(YEND, D, 9);

public:
	static bool init();

	static bool isAtFront(){
		return triggered[0];
	}

	static bool isAtEnd(){
		return triggered[1];
	}

	static void clearFrontSensor(){
		triggered[0] = false;
	}

	static void clearEndSensor(){
		triggered[1] = false;
	}

	//get the real states
	static bool getFrontState(){
		if(axis == X){
			return XFRONT::read();//state[0];
		}else{
			return YFRONT::read();
		}
	}

	static bool getEndState(){
		if(axis == X){
			return XEND::read();//state[1];
		}else{
			return YEND::read();
		}
	}

	//set real state, called by interrupt routine
	//if new state is false, triggered will be set too

	static void setFrontState(bool newState){
		//state[0] = newState;
		newState = !newState;
		//dout << "setfront " << axis << " newstate " << newState << " -> front " << triggered[0] << endl;
		if(newState){
			triggered[0] = true;
			triggeredPosition[0] = Control<axis>::getIsPosition();
		}
	}

	static void setEndState(bool newState){
		//state[1] = newState;
		newState = !newState;
		//dout << "setend " << axis << " newstate " << newState << " -> end " << triggered[1] << endl;
		if(newState){
			triggered[1] = true;
			triggeredPosition[1] = Control<axis>::getIsPosition();
		}
	}

	/**
	 * If the trigger is set and the axis has been moved
	 * by at least HALLSENSOR_HYSTERESIS, the trigger is unset
	 * @return returns true, if trigger is unset
	 */
	static bool checkFrontState();

	/**
	 * If the trigger is set and the axis has been moved
	 * by at least HALLSENSOR_HYSTERESIS, the trigger is unset
	 * @return true, if trigger is unset
	 */
	static bool checkEndState();

};

template<Axis axis> volatile bool HallSensor<axis>::triggered[2];
template<Axis axis> volatile bool HallSensor<axis>::state[2];
template<Axis axis> volatile int HallSensor<axis>::triggeredPosition[2];

template<> bool HallSensor<ALL>::init();

#include "hallSensor_impl.h"

//#pragma GCC pop_options
#endif /* HALLSENSOR_H_ */
