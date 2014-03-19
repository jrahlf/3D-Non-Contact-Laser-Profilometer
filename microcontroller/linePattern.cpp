/*
 * linePattern.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: jonas
 */

#include "linePattern.h"
#include "transform.h"
#include "control.h"
#include "pattern.h"

namespace{
int startX,stopX;
int startY,stopY;
double maxSpeed;

static inline bool reachedPointX(){
	return abs(Control<X>::getShallPosition() - Control<X>::getIsPosition()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static inline bool reachedPointY(){
	return abs(Control<Y>::getShallPosition() - Control<Y>::getIsPosition()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

bool passedFirst;
}

void LinePattern::configure(Point* points){
	startX = (points[0].x);
	startY = (points[0].y);
	stopX = (points[1].x);
	stopY = (points[1].y);

	//Control<X>::setShallPosition(Control<X>::getIsPosition());
	//Control<Y>::setShallPosition(Control<Y>::getIsPosition());
	maxSpeed = Control<ALL>::getMaxSpeed();
	Control<ALL>::setMaxSpeed(1);
	Control<X>::setShallPosition(startX);
	Control<Y>::setShallPosition(startY);
	passedFirst = false;
}

void LinePattern::sample(){
	if(reachedPointX() && reachedPointY()){
		Control<ALL>::setMaxSpeed(maxSpeed);
		if(passedFirst){
			Pattern::done();
		}
		passedFirst = true;
		Control<X>::setShallPosition(stopX);
		Control<Y>::setShallPosition(stopY);
	}
}



