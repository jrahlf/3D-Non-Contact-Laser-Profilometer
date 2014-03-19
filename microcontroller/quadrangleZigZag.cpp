/*
 * RectangleZigZag.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: jonas
 */

#include "quadrangleZigZag.h"
#include "transform.h"
#include "Trigger.h"
#include <xpcc/workflow/timeout.hpp>

/**
 * ^
 * |
 * |
 * |
 * |
 * y x------>
 *
 *
 * ^-> ->
 * | | | |
 * | | | |
 * | | | |
 * | | | |
 * | | | |
 * | -> ->
 */

namespace{

inline int nextX(){
	return Control<X>::getShallPosition();
}

inline int nextY(){
	return Control<Y>::getShallPosition();
}
bool approachingX;
int yDirection;
int xDirection;
bool yIsLongSide = true;
}

int QuadrangleZigZag::startX;
int QuadrangleZigZag::stopX;
int QuadrangleZigZag::startY;
int QuadrangleZigZag::stopY;
int QuadrangleZigZag::samplingResolution;

bool QuadrangleZigZag::isAtEnd(){
	return abs(nextX()-stopX) <= RECTANGLE_ZIGZAG_THRESHOLD
			&& stopY - Control<Y>::getIsPosition() <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static inline bool reachedPointX(int x){
	return abs(x-nextX()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static inline bool reachedPointY(int y){
	return abs(y-nextY()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static inline bool reachedPointX(){
	return abs(Control<X>::getShallPosition() - Control<X>::getIsPosition()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static inline bool reachedPointY(){
	return abs(Control<Y>::getShallPosition() - Control<Y>::getIsPosition()) <= RECTANGLE_ZIGZAG_THRESHOLD;
}

static int i = 0;
static xpcc::Timeout<> timeout;
static bool timerRunning = false;
static int pointsCounter = 0;
static float maxSpeed;
static bool approachingStart = false;

#if 0
inline void QuadrangleZigZag::yLongSample(){
	int x = Control<X>::getIsPosition();
	int y = Control<Y>::getIsPosition();
	int nextPosY;
	int nextPosX;
	if(approachingX && reachedPointX(x)){
		//system should be in rest
		if(Control<X>::getIsSpeed() !=0 ){
			i = 0;
			return;
		}
		i++;
		if(i < 50000){
			return;
		}
		nextPosY = yDirection == 1 ? stopY : startY;
		yDirection *= -1;
		Control<Y>::setShallPosition(nextPosY);
		approachingX = false;
	}else if(!approachingX && reachedPointY(y)){
		nextPosX = nextX()+samplingResolution;
		Control<X>::setShallPosition(nextPosX);
		approachingX = true;
	}
}

#endif

#define ZIGZAG 1
#if ZIGZAG

inline void QuadrangleZigZag::xLongSample(){
	int x = Control<X>::getIsPosition();
	int y = Control<Y>::getIsPosition();
	int nextPosY;
	int nextPosX;
	if(approachingX && reachedPointX(x)){
		nextPosY = nextY()+samplingResolution;
		Control<Y>::setShallPosition(nextPosY);
		approachingX = false;
	}else if(!approachingX && reachedPointY(y)){
		if(!timerRunning){
			timeout.restart(RECTANGLE_ZIGZAG_TIMEOUT);
			timerRunning = true;
		}

		if(timeout.isExpired()){
			if(Control<Y>::getIsSpeed() !=0 ){
				i = 0;
				return;
			}
			i++;
			if(i < 50000){
				return;
			}
		}
		//timeout reached or system is in rest
		timerRunning = false;
		timeout.stop();
		nextPosX = xDirection == 1 ? stopX : startX;
		xDirection *= -1;
		Control<X>::setShallPosition(nextPosX);
		approachingX = true;
	}
}
#else

inline void QuadrangleZigZag::xLongSample(){
	int x = Control<X>::getIsPosition();
	int y = Control<Y>::getIsPosition();
	int nextPosY;
	int nextPosX;
	if(approachingX && reachedPointX(x)){
		Control<ALL>::setMaxSpeed(0.8);
		Trigger::enable(false);
		nextPosY = nextY()+samplingResolution;
		Control<Y>::setShallPosition(nextPosY);
		nextPosX = startX;
		Control<X>::setShallPosition(nextPosX);
		approachingX = false;
	}else if(!approachingX && reachedPointY(y) && reachedPointX(x)){
		//system should be in rest
		if(!timerRunning){
			timeout.restart(RECTANGLE_ZIGZAG_TIMEOUT);
			timerRunning = true;
		}

		if(timeout.isExpired()){
			if(Control<Y>::getIsSpeed() !=0 ){
				i = 0;
				return;
			}
			i++;
			if(i < 50000){
				return;
			}
		}
		//timeout reached or system is in rest
		timerRunning = false;
		timeout.stop();

		Control<ALL>::setMaxSpeed(maxSpeed);
		Trigger::enable(true);
		nextPosX = stopX;
		Control<X>::setShallPosition(nextPosX);
		approachingX = true;
	}
}

#endif



#if 0
inline void QuadrangleZigZag::yLongSample(){
	int x = Control<X>::getIsPosition();
	int y = Control<Y>::getIsPosition();
	int nextPosY;
	int nextPosX;
	if(approachingX && reachedPointX(x) && reachedPointY(y)){
		//system should be in rest
		if(!timerRunning){
			timeout.restart(RECTANGLE_ZIGZAG_TIMEOUT);
			timerRunning = true;
		}

		if(timeout.isExpired()){
			if(Control<X>::getIsSpeed() !=0 ){
				i = 0;
				return;
			}
			i++;
			if(i < 50000){
				return;
			}
		}
		//timeout reached or system is in rest
		timerRunning = false;
		timeout.stop();
		//Control<ALL>::setMaxSpeed(maxSpeed);
		Trigger::enable(true);
		nextPosY = stopY;
		Control<Y>::setShallPosition(nextPosY);
		approachingX = false;
	}else if(!approachingX && reachedPointY(y)){

		//Control<ALL>::setMaxSpeed(1);
		Trigger::enable(false);
		nextPosX = nextX()+samplingResolution;
		Control<X>::setShallPosition(nextPosX);
		nextPosY = startY;
		Control<Y>::setShallPosition(nextPosY);
		approachingX = true;
	}
}

#endif

void QuadrangleZigZag::sample(){
	if(isAtEnd() && Pattern::isRunning()){
		Pattern::done();
	}
	if(approachingStart){
		if(reachedPointX() && reachedPointY()){
			Control<ALL>::setMaxSpeed(maxSpeed);
			approachingStart  = false;
		}else{
			Control<ALL>::setMaxSpeed(1);
			return;
		}
	}

	xLongSample();

	//if(yIsLongSide){
	//	yLongSample();
	//}else{
	//	xLongSample();
	//}
	//sit back and let control do its job
}

void QuadrangleZigZag::start(){
	maxSpeed = Control<ALL>::getMaxSpeed();
	dout << "pattern max speed: " << maxSpeed << endl;
	Trigger::enable(false);
	yDirection = 1;
	xDirection = 1;
	yIsLongSide = (stopY - startY) > (stopX - startX);
	yIsLongSide = false;
	approachingX = yIsLongSide;
	approachingStart = true;
	Control<X>::setShallPosition(startX);
	Control<Y>::setShallPosition(startY);
}

void QuadrangleZigZag::configure(float x, float y, float width, float height, int samplingResolution){
	QuadrangleZigZag::startX = Transform::mmToEncoder(x);
	QuadrangleZigZag::startY = Transform::mmToEncoder(y);
	QuadrangleZigZag::stopX = startX + Transform::mmToEncoder(width);
	QuadrangleZigZag::stopY = startY + Transform::mmToEncoder(height);
	QuadrangleZigZag::samplingResolution = samplingResolution;
	start();
}

void QuadrangleZigZag::configure2(float startX, float startY, float stopX, float stopY, int samplingResolution){
	QuadrangleZigZag::startX = Transform::mmToEncoder(startX);
	QuadrangleZigZag::startY = Transform::mmToEncoder(startY);
	QuadrangleZigZag::stopX = Transform::mmToEncoder(stopX);
	QuadrangleZigZag::stopY = Transform::mmToEncoder(stopY);
	QuadrangleZigZag::samplingResolution = samplingResolution;
	start();
}

void QuadrangleZigZag::configure(Point points[4], int samplingResolution){
	QuadrangleZigZag::samplingResolution = samplingResolution;
	startX = XSTAGE_LENGTH_ENCODER;
	startY = YSTAGE_LENGTH_ENCODER;
	stopX = 0;
	stopY = 0;

	for(int i = 0; i < 4; i++){
		int x = points[i].x;
		int y = points[i].y;
		if(x < startX){
			startX = x;
		}
		if(x > stopX){
			stopX = x;
		}
		if(y < startY){
			startY = y;
		}
		if(y > stopY){
			stopY = y;
		}
	}

	start();
}
