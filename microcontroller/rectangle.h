/*
 * rectangle.h
 *
 *  Created on: Feb 1, 2014
 *      Author: jonas
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "pattern.h"

class Quadrangle{
private:
	static int x, y, width, height;

public:
	/**
	 * Units in mm
	 * @param x
	 * @param y
	 * @param width
	 * @param height
	 */
	static void configure(float x, float y, float width, float height);
	static void configure(Point points[4]);
	static void sample();
};



#endif /* RECTANGLE_H_ */
