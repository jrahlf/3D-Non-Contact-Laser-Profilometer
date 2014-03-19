/*
 * RectangleZigZag.h
 *
 *  Created on: Jan 18, 2014
 *      Author: jonas
 */

#ifndef RECTANGLEZIGZAG_H_
#define RECTANGLEZIGZAG_H_

#include "pattern.h"
#include "Trigger.h"

class QuadrangleZigZag{
private:
	static int startX, stopX, startY, stopY;
	static int samplingResolution;

	static bool isAtEnd();
	static void start();
	static void yLongSample();
	static void xLongSample();
public:
	/**
	 * Size in mm
	 * samplingResolution in encoder counts
	 */
	static void configure(float x, float y, float width, float height, int samplingResolution);
	/**
	 * Size in mm
	 * samplingResolution in encoder counts
	 */
	static void configure2(float startX, float startY, float stopX, float stopY, int samplingResolution);
	/**
	 * Point coordinates in encoder counts
	 * samplingResolution in encoder counts
	 */
	static void configure(Point points[4], int samplingResolution = Trigger::getThreshold());
	static void sample();
};

#endif /* RECTANGLEZIGZAG_H_ */
