/*
 * project.h
 *
 *  Created on: Dec 29, 2013
 *      Author: jonas
 */

#ifndef PROJECT_H_
#define PROJECT_H_


enum Axis{
	X = 0,
	Y = 2,
	Z = 4, //unused
	ALL
};

struct Point{
	int x,y;
};


#ifdef __arm__
#include <xpcc/architecture.hpp>

/**
 * position-out, similar to mout, but only contains position data
 * Prefix character: P
 */
extern xpcc::IOStream pout;
/**
 * measurement-out, for sending measurement data to computer
 * Prefix character: M
 * Raw stream like coutRaw, to reduce latency and prevent inconsistency
 * between position and measurement
 */
extern xpcc::IOStream mout;
/**
 * debug-out, for printing debug statements
 * should be disabled for final software
 * Prefix character: D
 */
extern xpcc::IOStream dout;
/**
 * for error logging
 * Prefix character: E
 */
extern xpcc::IOStream cerr;
/**
 * INFO-out, for printing important informations
 * and for printing requested values (e.g. getLaserError)
 * Prefix character: I
 */
extern xpcc::IOStream iout;
/**
 * COMMAND-out, for sending commands from microcontroller
 * to computer
 * Prefix character: C
 */
extern xpcc::IOStream commandOut;
/**
 * CONTROL-out, for dumping position/velocity control
 * Prefix character: K
 */
extern xpcc::IOStream controlOut;
/**
 * for debugging, no prefix
 * Not to be used with final software
 */
extern xpcc::IOStream coutRaw;
#endif
#define endl '\n'

// --------------------------------------------------------------------
// Motor related constants
// --------------------------------------------------------------------
const int MOTOR_RPM = 5500;
const int MOTOR_ENC_PER_REV = 400;
const int MOTOR_GEAR_RATIO = 5;
//datasheet: 36666
//experimental: 49000
const int MOTOR_MAX_SPEED = 49000; //encoder counts per second
#define MOTOR_MAX_VOLTAGE 12


// --------------------------------------------------------------------
// CONTROL RELATED CONSTANTS
// CONTROLX refers to position control
// --------------------------------------------------------------------
#define CONTROL_FREQUENCY 1000
const float CONTROLV_KP = 0.2f;
const float CONTROLV_KI = 0.1f;
const float CONTROLV_KD = 0;
const float CONTROLV_MAX_OUTPUT = 1200;
const float CONTROLV_MAX_ERRORSUM = MOTOR_MAX_SPEED/5;

const float CONTROLX_KP = 10.0f;
const float CONTROLX_KI = 0.1f;
const float CONTROLX_KD = 0.3f;
const float CONTROLX_MAX_OUTPUT = MOTOR_MAX_SPEED;
const float CONTROLX_MAX_ERRORSUM = MOTOR_ENC_PER_REV*10;
const int CONTROLX_DEADBAND = 0;


// --------------------------------------------------------------------
// Rectangle zig-zag pattern related constants
// --------------------------------------------------------------------
const int RECTANGLE_ZIGZAG_THRESHOLD = CONTROLX_DEADBAND+1;
const int RECTANGLE_ZIGZAG_TIMEOUT = 500; //[ms]


// --------------------------------------------------------------------
// Security controller related constants
// --------------------------------------------------------------------
#define HALLSENSOR_HYSTERESIS 5000

// --------------------------------------------------------------------
// Laser interface related constants
// --------------------------------------------------------------------
#define LASER_INPUT_BUFFER_SIZE 50
#define INVALID_MEASUREMENT_STRING "9000"
#define USE_LASER_DMA 0

// --------------------------------------------------------------------
// X-Y stage related constants
// --------------------------------------------------------------------
//TODO
const int YSTAGE_LENGTH_MM = 100;
const int XSTAGE_LENGTH_MM = 100;
const int XSTAGE_LENGTH_ENCODER = XSTAGE_LENGTH_MM*MOTOR_ENC_PER_REV*MOTOR_GEAR_RATIO;
const int YSTAGE_LENGTH_ENCODER = YSTAGE_LENGTH_MM*MOTOR_ENC_PER_REV*MOTOR_GEAR_RATIO;

#endif /* PROJECT_H_ */
