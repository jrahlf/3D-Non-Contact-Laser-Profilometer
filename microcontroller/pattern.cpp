/*
 * pattern.cpp
 *
 *  Created on: Jan 20, 2014
 *      Author: jonas
 */

#include "pattern.h"

void (*Pattern::func)(void) = 0;
bool Pattern::running = false;
bool Pattern::paused = false;
