/*
 * rectangle.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: jonas
 */

#include "rectangle.h"
#include "transform.h"
#include "control.h"

int Quadrangle::x;
int Quadrangle::y;
int Quadrangle::width;
int Quadrangle::height;



static Point points[5];
static int pointsIndex = 0;

static bool reachedPoint(){
	return abs(Control<X>::getIsPosition() - points[pointsIndex].x) < 30 &&
			abs(Control<Y>::getIsPosition() - points[pointsIndex].y) < 30;
}


void Quadrangle::configure(float x, float y, float width, float height){
	Quadrangle::x = Transform::mmToEncoder(x);
	Quadrangle::y = Transform::mmToEncoder(y);
	Quadrangle::width = Transform::mmToEncoder(width);
	Quadrangle::height = Transform::mmToEncoder(height);

	/**
	 * 2-----3
	 * |     |
	 * |     |
	 * 1-----4
	 */

	points[0].x = Quadrangle::x;
	points[0].y = Quadrangle::y;

	points[1].x = points[0].x;
	points[1].y = points[0].y + Quadrangle::height;

	points[2].x = points[1].x + Quadrangle::width;
	points[2].y = points[1].y;

	points[3].x = points[2].x;
	points[3].y = points[0].y;

	points[4].x = points[0].x;
	points[4].y = points[0].y;
	pointsIndex = 0;

	Control<X>::setShallPosition(points[0].x);
	Control<Y>::setShallPosition(points[0].y);
	dout << "first point x " << Control<X>::getShallPosition() << endl;
	dout << "first point y " << Control<Y>::getShallPosition() << endl;
}

void Quadrangle::sample(){
	if(reachedPoint()){
		pointsIndex++;
		if(pointsIndex > 4){
			Pattern::done();
			dout << "rectangle pattern is done " << endl;
			return;
		}
		Control<X>::setShallPosition(points[pointsIndex].x);
		Control<Y>::setShallPosition(points[pointsIndex].y);
	}

}

