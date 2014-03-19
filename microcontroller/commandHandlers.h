/*
 * commandHandlers.h
 *
 *  Created on: Jan 23, 2014
 *      Author: jonas
 */

#ifndef COMMANDHANDLERS_H_
#define COMMANDHANDLERS_H_



#include <xpcc/architecture.hpp>
#include "project.h"
#include "control.h"
#include "atof.h"
#include "limits.h"
#include "transform.h"
#include "motor.h"
#include "laserInterface.h"
#include "Trigger.h"
#include "../ext/stm32f4xx_lib/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include "utils.h"
#include "pattern.h"
#include "rectangle.h"
#include "quadrangleZigZag.h"
#include "linePattern.h"


struct Handlers{

template<Axis axis>
static bool gotoHandler(const char* cmd, int length){
	float pos = atof2(cmd, length, 99999.0f);
	if(pos == 99999.0f){
		return false;
	}
	Utils::startLoggingControl();
	pos = Transform::mmToEncoder(pos);
	Control<ALL>::enablePositionControl(true);
	Control<axis>::setShallPosition(pos);
	dout << axis << " going to " << pos << endl;
	return true;
}

template<Axis axis>
static bool moveHandler(const char* cmd, int length){
	float pos = atof2(cmd, length, 99999.0f);
	if(pos == 99999.0f){
		return false;
	}
	Utils::startLoggingControl();
	pos = Transform::mmToEncoder(pos);
	Control<ALL>::enablePositionControl(true);
	pos = Control<axis>::getIsPosition() + pos;
	Control<axis>::setShallPosition(pos);
	dout << axis << " going to " << pos << endl;
	return true;
}

template<Axis axis>
static bool setSpeedHandler(const char* cmd, int length){
	float speed = atof2(cmd, length, 2.f);
	if(speed == 2.f){
		cerr << "speed param must be between -1 and 1" << endl;
		return false;
	}
	Utils::startLoggingControl();
	Control<ALL>::enablePositionControl(false);
	Control<ALL>::enableSpeedControl(true);
	Control<axis>::setShallSpeed(speed);
	//dout << axis << " set speed to " << speed << endl;
	return true;
}

static bool setMaxSpeedHandler(const char* cmd, int length){
	float speed = atof2(cmd, length, 2.f);
	if(speed == 2.f){
		cerr << "speed param must be between 0 and 1" << endl;
		return false;
	}
	Control<ALL>::setMaxSpeed(speed);
	dout << " set speed to " << speed << endl;
	return true;
}

template<Axis axis>
static bool setPwmHandler(const char* cmd, int length){
	int speed = atoi2(cmd, length, INT_MAX);
	if(speed == INT_MAX){
		return false;
	}
	xpcc::stm32::SysTickTimer::enable();
	xpcc::delay_ms(5);
	if(speed > 1200){
		speed = 1200;
	}else if(speed < -1200){
		speed = -1200;
	}
	Control<ALL>::enablePositionControl(false);
	Control<ALL>::enableSpeedControl(false);
	Motor<axis>::setSpeed(speed);
	dout << axis << " set pwm to " << speed << endl;
	return true;
}

static bool enableSystickHandler(const char* cmd, int length){
	(void) cmd;
	(void) length;
	bool enable = atoi2(cmd, 1, 1);
	if(enable){
		Utils::enableSystick();
	}else{
		Utils::disableSystick();
	}
	dout << "systick enable " << enable << endl;
	return true;
}

static bool enableTrigger(const char* cmd, int length){
	(void) cmd;
	(void) length;
	bool param = atoi2(cmd, 1, 2);
	Trigger::enable(param);
	dout << "trigger enable " << param << endl;
	return true;
}

static bool getLaserError(const char* cmd, int length){
	(void) cmd;
	(void) length;
	iout << "Laser error: " << Laser::getLastError() << endl;
	return false;
}

static bool resetHandler(const char* cmd, int length){
	(void) cmd;
	(void) length;
	NVIC_SystemReset();
	return true;
}

static bool pausePatternHandler(const char* cmd, int length){
	(void) cmd;
	(void) length;
	Pattern::pause();
	return true;
}

static bool resumePatternHandler(const char* cmd, int length){
	(void) cmd;
	(void) length;
	Pattern::resume();
	return true;
}

static bool stopPatternHandler(const char* cmd, int length){
	(void) cmd;
	(void) length;
	Pattern::done();
	return true;
}

static bool getPushButtonStates(const char* cmd, int length){
	(void) cmd;
	(void) length;
	iout << "real states: X: " << HallSensor<X>::getFrontState() << "-" << HallSensor<X>::getEndState();
	iout << " :: Y: " << HallSensor<Y>::getFrontState() << "-" << HallSensor<Y>::getEndState() << endl;
	iout << "buffered states: " << HallSensor<X>::isAtFront() << "-" << HallSensor<X>::isAtEnd();
	iout << " :: " << HallSensor<Y>::isAtFront() << "-" << HallSensor<Y>::isAtEnd() << endl;
	return false;
}

static bool getMeasurement(const char* cmd, int length){
	(void) cmd;
	(void) length;
	Trigger::forceTrigger();
	return false;
}

static bool execLaserCommand(const char* cmd, int length){
	//TODO
	(void) length;
	const char* result = Laser::exec(cmd);
	iout << "EXEC: " << result << endl;
	return false;
}

static bool mirrorCommand(const char* cmd, int length){
	//TODO
	(void) cmd;
	(void) length;
	if(length < 1){
		iout << "no command found to mirror" << endl;
		return false;
	}
	commandOut << cmd << endl;
	return false;
}

static bool setTriggerThreshold(const char* cmd, int length){
	float param = atof2(cmd, length, 0);
	if(param == 0){
		iout << "invalid argument for trigger threshold" << endl;
		return false;
	}
	int threshold = Transform::mumToEncoder(param);
	Trigger::setThreshold(threshold);
	return true;
}

template<Axis axis>
static bool getControlOutputs(const char* cmd, int length){
	(void) cmd;
	(void) length;
	iout << "w_x x w_v v v_u v_ureal" << endl;
	iout << Control<axis>::getShallPosition() << " " << Control<axis>::getIsPosition() << " "
				<< Control<axis>::getShallSpeed() << " " << Control<axis>::getIsSpeed() << " "
				<< Control<axis>::getSpeedU() << " " << Control<axis>::getSpeedRealU() << endl;
	iout << "posPid enabled, vPid enabled" << endl;
	iout << Control<axis>::isPositionControlEnabled() << ", " << Control<axis>::isSpeedControlEnabled() << endl;
	return false;
}

static bool rectanglePatternHandler3(const char* cmd, int length){
	static int i = 0;
	static Point points[5];

	if(Pattern::isRunning()){
		iout << "Previous pattern is still running!" << endl;
		return false;
	}

	points[i].x = Control<X>::getIsPosition();
	points[i].y = Control<Y>::getIsPosition();
	dout << "point added (" << points[i].x << ", " << points[i].y << ") i " << i << endl;
	i++;

	if(i < 4){
		return false;
	}

	if(i == 5){
		Pattern::start();
		i = 0;
	}

	Utils::enableSystick();
	Control<ALL>::enablePositionControl(true);
	//execute pattern
	QuadrangleZigZag zRect;
	zRect.configure(points);
	Pattern::setPattern(zRect);
	//Trigger::setThreshold(samplingResolution);
	Trigger::enable(true);
	iout << "pattern recorded, send command once more to start it" << endl;
	//Pattern::start();
	//i = 0;
	return false;
}

static bool linePatternHandler(const char* cmd, int length){
	static int i = 0;
	static Point points[3];

	if(Pattern::isRunning()){
		iout << "Previous pattern is still running!" << endl;
		return false;
	}

	points[i].x = Control<X>::getIsPosition();
	points[i].y = Control<Y>::getIsPosition();
	dout << "point added (" << points[i].x << ", " << points[i].y << ") i " << i << endl;
	i++;

	if(i < 2){
		return false;
	}

	if(i == 3){
		//dout << "starting pattern" << endl;
		Pattern::start();
		i = 0;
		return true;
	}
	//dout << "setting up pattern" << endl;

	Utils::enableSystick();
	Control<ALL>::enablePositionControl(true);
	//execute pattern
	LinePattern line;
	line.configure(points);
	Pattern::setPattern(line);
	Trigger::enable(true);
	iout << "pattern recorded, send command once more to start it" << endl;
	//Pattern::start();
	//i = 0;
	return false;
}

static bool rectanglePatternHandler2(const char* cmd, int length){
	//expected: 4 arguments: start x, start y, stop x, stop y
	static float lastx=-1, lasty=-1, lastStopX=-1, lastStopY=-1;

	const char* currentPos;
	float startx = atof2(cmd, &currentPos, 100000.f);
	float starty = atof2(currentPos, &currentPos, 100000.f);
	float stopx = atof2(currentPos, &currentPos, 100000.f);
	float stopy = atof2(currentPos, &currentPos, 100000.f);
	int samplingResolution = atoi2(currentPos, &currentPos);

	//sanity checks:
	if(currentPos - cmd > length){
		cerr << "buffer overrun " << (currentPos - cmd) << endl;
		return false;
	}
	if(startx > XSTAGE_LENGTH_MM || startx < 0){
		iout << "x param is out of bounds: " << startx << endl;
		return false;
	}
	if(starty > YSTAGE_LENGTH_MM || stopy < 0){
		iout << "y param is out of bounds: " << stopy << endl;
		return false;
	}
	if(stopx > XSTAGE_LENGTH_MM || stopx < startx){
		iout << "stopx param is out of bounds: " << stopx << endl;
		return false;
	}
	if(stopy > YSTAGE_LENGTH_MM || stopy < starty){
		iout << "stopy param is out of bounds: " << starty << endl;
		return false;
	}
	if(samplingResolution < CONTROLX_DEADBAND || samplingResolution > 100000){
		iout << "sampling resolution param is out of bounds: " << samplingResolution << endl;
		return false;
	}

	dout << "rectangle: (startx, starty, stopx, stopy) "
			<< startx << ", " << starty << ", " << stopx << ", " << stopy << endl;

	if(Pattern::isRunning()){
		iout << "Previous pattern is still running!" << endl;
		return false;
	}

	Control<ALL>::setMaxSpeed(0.05);
	Utils::enableSystick();
	Control<ALL>::enablePositionControl(true);
	if( true || (startx == lastx && starty == lasty && stopx == lastStopX && stopy == lastStopY)){
		//execute pattern
		Trigger::enable(true);
		QuadrangleZigZag zRect;
		zRect.configure2(startx, starty, stopx, stopy, samplingResolution);
		Pattern::setPattern(zRect);
		Trigger::setThreshold(samplingResolution);
		Trigger::enable(true);
		Pattern::start();
		return true;
	}

	lastx = startx;
	lasty = starty;
	lastStopX = stopx;
	lastStopY = stopy;

	//demonstrate pattern outline
	Trigger::enable(true);
	Quadrangle rect;
	rect.configure(startx, starty, stopx-startx, stopy-starty);
	Pattern::setPattern(rect);
	Pattern::start();

	return true;
}

static bool rectanglePatternHandler(const char* cmd, int length){
	//expected: 4 arguments: x, y, width, height
	static float lastx=-1, lasty=-1, lastw=-1, lasth=-1;

	const char* currentPos;
	float x = atof2(cmd, &currentPos, 100000.f);
	float y = atof2(currentPos, &currentPos, 100000.f);
	float width = atof2(currentPos, &currentPos, 100000.f);
	float height = atof2(currentPos, &currentPos, 100000.f);
	int samplingResolution = atoi2(currentPos, &currentPos);

	//sanity checks:
	if(currentPos - cmd > length){
		cerr << "buffer overrun " << (currentPos - cmd) << endl;
		return false;
	}
	if(x > XSTAGE_LENGTH_MM || x < 0){
		iout << "x param is out of bounds: " << x << endl;
		return false;
	}
	if(y > YSTAGE_LENGTH_MM || y < 0){
		iout << "y param is out of bounds: " << y << endl;
		return false;
	}
	if(x+width > XSTAGE_LENGTH_MM || width <= 0){
		iout << "(x+)width param is out of bounds: " << (x+width) << endl;
		return false;
	}
	if(y+height > YSTAGE_LENGTH_MM || height <= 0){
		iout << "(y+)height param is out of bounds: " << height << endl;
		return false;
	}
	if(samplingResolution < CONTROLX_DEADBAND || samplingResolution > 100000){
		iout << "sampling resolution param is out of bounds: " << samplingResolution << endl;
		return false;
	}

	dout << "rectangle: (x,y,width,height) " << x << ", " << y << ", " << width << ", " << height << endl;

	if(Pattern::isRunning()){
		iout << "Previous pattern is still running!" << endl;
		return false;
	}
	Utils::enableSystick();
	Control<ALL>::enablePositionControl(true);
	if(x == lastx && y == lasty && width == lastw && height == lasth){
		//execute pattern
		Trigger::enable(true);
		QuadrangleZigZag zRect;
		zRect.configure(x, y, width, height, samplingResolution);
		Pattern::setPattern(zRect);
		Trigger::setThreshold(samplingResolution);
		Trigger::enable(true);
		Pattern::start();
		return true;
	}

	lastx = x;
	lasty = y;
	lastw = width;
	lasth = height;

	//demonstrate pattern outline
	Trigger::enable(true);
	Quadrangle rect;
	rect.configure(x, y, width, height);
	Pattern::setPattern(rect);
	Pattern::start();

	return true;
}

};

#endif /* COMMANDHANDLERS_H_ */
