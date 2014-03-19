/*
 * utils.h
 *
 *  Created on: Feb 1, 2014
 *      Author: jonas
 */

#ifndef UTILS_H_
#define UTILS_H_

namespace Utils{
	void enableSystick();
	void disableSystick();
	void startLoggingControl();
	/**
	 * Drives to position (0,0), that is the most left and most lower
	 * position of the stage, when viewed from the top
	 * When that position is reached, the current position is set to 0
	 * To be called at startup
	 */
	void calibrateZero();
};



#endif /* UTILS_H_ */
