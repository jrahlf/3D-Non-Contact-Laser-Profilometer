/*
 * pattern.h
 *
 *  Created on: Jan 18, 2014
 *      Author: jonas
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include "control.h"
#include "project.h"
#include "state.h"
#include "pcInterface.h"

class QuadrangleZigZag;

class Pattern{
	static void (*func)(void);
	static bool running;
	static bool paused;
public:
	template<class T>
	static void setPattern(T pattern, float maxSpeed = 1){
		(void)pattern;
		//Control<ALL>::setMaxSpeed(maxSpeed);
		func = &T::sample;
	}
	static void follow(){
		if(!running || func == 0){
			return;
		}
		func();
	}
	static void pause(){
		running = false;
		paused = true;
	}
	static void start(){
		running = true;
		paused = false;
		State::set(State::SCANNING_RECTANGLE);
		dout << "Pattern start" << endl;
		commandOut << PC::ucCommand::FLUSH_DATA << endl;
	}
	static void resume(){
		running = true;
		/*if(!paused){
			start();
		}*/
		paused = false;
	}
	static void done(){
		running = 0;
		State::set(State::READY);
		dout << "Pattern done" << endl;
	}
	static int isRunning(){
		return running && !paused;
	}
};



#endif /* PATTERN_H_ */
