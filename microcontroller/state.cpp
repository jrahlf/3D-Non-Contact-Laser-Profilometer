/*
 * state.cpp
 *
 *  Created on: Dec 30, 2013
 *      Author: jonas
 */

#include "state.h"
#include "project.h"
#include "pcInterface.h"

#include <xpcc/architecture.hpp>

using namespace xpcc::stm32;

GPIO__OUTPUT(ledRed, D, 14);
GPIO__OUTPUT(ledBlue, D, 15);
GPIO__OUTPUT(ledOrange, D, 13);
GPIO__OUTPUT(ledGreen, D, 12);

int State::currentState;


void disableAllLeds(){

	//disable timers
	Timer1::pause();
	Timer6::pause();
	Timer7::pause();

	ledRed::set(false);
	ledBlue::set(false);
	ledOrange::set(false);
	ledGreen::set(false);
}

void State::init(){
	ledRed::setOutput(xpcc::stm32::PUSH_PULL);
	ledBlue::setOutput(xpcc::stm32::PUSH_PULL);
	ledOrange::setOutput(xpcc::stm32::PUSH_PULL);
	ledGreen::setOutput(xpcc::stm32::PUSH_PULL);

	//configure timers:
	//timer1 -> red led -> errors
	Timer1::enable();
	Timer1::setMode(Timer3::UP_COUNTER);
	Timer1::enableInterruptVector(GeneralPurposeTimer::Interrupt::INTERRUPT_UPDATE, true, 15);
	Timer1::enableInterrupt(Timer1::INTERRUPT_UPDATE);
	Timer1::setPeriod(300000);	// 300 ms

	//timer6 -> blue led -> executing rectangle scan
	Timer6::enable();
	Timer6::setMode(Timer6::UP_COUNTER);
	Timer6::enableInterruptVector(true, 15);
	Timer6::enableInterrupt(Timer6::INTERRUPT_UPDATE);
	Timer6::setPeriod(1000000);	// 1000 ms

	//timer7 -> orange led -> velocity controlled
	Timer7::enable();
	Timer7::setMode(Timer7::UP_COUNTER);
	Timer7::enableInterruptVector(true, 15);
	Timer7::enableInterrupt(Timer7::INTERRUPT_UPDATE);
	Timer7::setPeriod(200000);	// 700 ms

	State::set(State::INITIALIZING);
}

void State::set(int newState){
	currentState = newState;
	disableAllLeds();

	dout << "going to state " << newState << endl;
	switch(newState){
	case State::INITIALIZING:
		ledOrange::set(true);
		ledRed::set(true);
		break;
	case State::READY:
		ledGreen::set(true);
		break;
	case State::SCANNING_RECTANGLE:
		Timer6::start();
		break;
	case State::MOTOR1_FAULT:
	case State::MOTOR2_FAULT:
	case State::PC_INIT_FAIL:
		Timer1::start();
		break;
	}
}

void State::setSecondaryState(int state, bool set){
	//dout << "motor limit " << set << endl;
	switch(state){
	case State::MOTOR_LIMITED:
		if(set){
			Timer7::start();
		}else{
			Timer7::pause();
			setOrange(false);
		}
		break;
	}
}

void State::setRed(bool on){
	ledRed::set(on);
}

void State::setBlue(bool on){
	ledBlue::set(on);
}

void State::setOrange(bool on){
	ledOrange::set(on);
}

void State::setGreen(bool on){
	ledGreen::set(on);
}

extern "C" void
TIM1_UP_TIM10_IRQHandler(void){
	Timer1::resetInterruptFlags(Timer1::FLAG_UPDATE);
	ledRed::toggle();
}

extern "C" void
TIM6_DAC_IRQHandler(void){
	Timer6::resetInterruptFlags(Timer6::FLAG_UPDATE);
	ledBlue::toggle();
}

extern "C" void
TIM7_IRQHandler(void){
	Timer7::resetInterruptFlags(Timer7::FLAG_UPDATE);
	ledOrange::toggle();
}


