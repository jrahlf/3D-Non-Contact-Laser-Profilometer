/*
 * state.h
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 */

#ifndef STATE_H_
#define STATE_H_

class State{
public:

	static const int INITIALIZING = 0x0;
	static const int READY = 0x1;
	static const int MOTOR_LIMITED = 0x1<<1;
	static const int SCANNING_RECTANGLE = 0x1<<2;

	static const int PC_INIT_FAIL = 0x1<<3;
	static const int MOTOR1_FAULT = 0x1<<4;
	static const int MOTOR2_FAULT = 0x1<<5;

	/**
	 * This should be the very first function call
	 * after setting up the cpu frequency
	 */
	static void init();

	/**
	 *
	 * @return the current state
	 */
	static int get(){
		return currentState;
	}

	/**
	 * Sets the new state
	 * @param newState the new state
	 * TODO send message to PC in case of errors
	 */
	static void set(int newState);
	static void setSecondaryState(int state, bool set);
	static void setGreen(bool on);
	static void setRed(bool on);
	static void setBlue(bool on);
	static void setOrange(bool on);

private:
	static int currentState;
};


#endif /* STATE_H_ */
