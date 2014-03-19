/*
 * Trigger.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: jonas
 */

#include "Trigger.h"
#include "laserInterface.h"
#include "control.h"

namespace{
int thresholdX = 20;
int thresholdY = 20;
int lastXPos = 0;
int lastYPos = 0;
bool enabled = true;
}


void Trigger::sample(){
	if(!enabled){
		return;
	}

	int xnow = Control<X>::getIsPosition();
	int ynow = Control<Y>::getIsPosition();

	if(abs(xnow - lastXPos) >= thresholdX ||
	   abs(ynow - lastYPos) >= thresholdY){
		//send output
		lastXPos = xnow;
		lastYPos = ynow;
		if(Laser::DoubleBuffer::good()){
			mout << xnow << "," << ynow << ",";
			//measurement is exactly 8 chars long
			const char* p = Laser::DoubleBuffer::get()+3;
			for(int i = 0; i < 8; i++){
				mout << *p;
				p++;
			}
			mout << endl;
		}else{
			pout << xnow << "," << ynow << endl;
		}
	}
}

void Trigger::forceTrigger(){

	int xnow = Control<X>::getIsPosition();
	int ynow = Control<Y>::getIsPosition();
		//send output
	lastXPos = xnow;
	lastYPos = ynow;

	if(Laser::DoubleBuffer::good()){
		mout << xnow << "," << ynow << ",";
		//measurement is exactly 8 chars long
		const char* p = Laser::DoubleBuffer::get()+3;
		for(int i = 0; i < 8; i++){
			mout << *p;
			p++;
		}
		mout << endl;
	}else{
		pout << xnow << "," << ynow << endl;
	}

}

void Trigger::setThreshold(int threshold){
	thresholdX = threshold;
	thresholdY = threshold;
}

int Trigger::getThreshold(){
	return thresholdX;
}

void Trigger::enable(bool enable){
	enabled = enable;
}
