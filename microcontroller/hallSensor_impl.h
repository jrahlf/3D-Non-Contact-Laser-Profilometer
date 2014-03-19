/*
 * hallSensor_impl.h
 *
 *  Created on: Jan 1, 2014
 *      Author: jonas
 */

#ifndef HALLSENSOR_IMPL_H_
#define HALLSENSOR_IMPL_H_

#include "project.h"

template<Axis axis>
bool HallSensor<axis>::checkFrontState(){
	//coutRaw << "a " << Control<axis>::getIsPosition() - triggeredPosition[0] << endl;
	if(Control<axis>::getIsPosition() - triggeredPosition[0] > HALLSENSOR_HYSTERESIS){
		triggered[0] = false;
		return true;
	}
	return false;
}

template<Axis axis>
bool HallSensor<axis>::checkEndState(){
	//coutRaw << "b " << Control<axis>::getIsPosition() - triggeredPosition[0] << endl;
	if(Control<axis>::getIsPosition() - triggeredPosition[1] < -HALLSENSOR_HYSTERESIS){
		triggered[1] = false;
		return true;
	}
	return false;
}


#endif /* HALLSENSOR_IMPL_H_ */
