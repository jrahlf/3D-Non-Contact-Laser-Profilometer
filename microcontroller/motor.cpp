/*
 * motor.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: jonas
 */

#include "motor.h"
#include <xpcc/architecture.hpp>
#include "project.h"

using namespace xpcc::stm32;

//private namespace
namespace{
GPIO__INPUT(M1_FB, B, 0);
GPIO__INPUT(M2_FB, B, 1);
GPIO__INPUT(M1_FS, B, 2);
GPIO__INPUT(M2_FS, B, 4);

GPIO__OUTPUT(M1_IN1, C, 6); //TIM3 CH1
GPIO__OUTPUT(M1_IN2, C, 7); //TIM3 CH2
GPIO__OUTPUT(M2_IN1, C, 8); //TIM3 CH3
GPIO__OUTPUT(M2_IN2, C, 9); //TIM3 CH4
}

void saturate(int& value){
	if(value > 1200){
		value = 1200;
	}else if(value < -1200){
		value = -1200;
	}
	/*if(abs(value) < 50){
		value = 0;
	}*/
}


template<>
void Motor<X>::brake(){
	Timer3::setCompareValue(1, 0);
	Timer3::setCompareValue(2, 0);
}

template<>
void Motor<Y>::brake(){
	Timer3::setCompareValue(3, 0);
	Timer3::setCompareValue(4, 0);
}

template<>
void Motor<ALL>::brake(){
	Motor<X>::brake();
	Motor<Y>::brake();
}

template<>
void Motor<X>::setSpeed(int speed){
	saturate(speed);
	if(speed > 0){
		Timer3::setCompareValue(1, speed);
		Timer3::setCompareValue(2, 0);
	}
	else{
		speed = -speed;
		Timer3::setCompareValue(2, speed);
		Timer3::setCompareValue(1, 0);
	}
}

template<>
void Motor<Y>::setSpeed(int speed){
	saturate(speed);
	if(speed > 0){
		Timer3::setCompareValue(3, speed);
		Timer3::setCompareValue(4, 0);
	}
	else{
		speed = -speed;
		Timer3::setCompareValue(4, speed);
		Timer3::setCompareValue(3, 0);
	}
}

template<>
bool Motor<ALL>::init(){
	//TODO Feedback

	M1_FS::setInput(xpcc::stm32::PULLUP);
	ExtInt::enable(ExtInt::Pin::PB2);
	ExtInt::setMode(ExtInt::Pin::PB2, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PB2);
	ExtInt::enableInterruptVector(ExtInt::Pin::PB2, 15);

	M2_FS::setInput(xpcc::stm32::PULLUP);
	ExtInt::enable(ExtInt::Pin::PB4);
	ExtInt::setMode(ExtInt::Pin::PB4, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PB4);
	ExtInt::enableInterruptVector(ExtInt::Pin::PB4, 15);

	M1_IN1::setAlternateFunction(AF_TIM3, PUSH_PULL);
	M1_IN2::setAlternateFunction(AF_TIM3, PUSH_PULL);
	M2_IN1::setAlternateFunction(AF_TIM3, PUSH_PULL);
	M2_IN2::setAlternateFunction(AF_TIM3, PUSH_PULL);

	Timer3::enable();
	Timer3::setMode(Timer3::UP_COUNTER);

	// 72 MHz / 2 / 1200 = 60kHz
	Timer3::setPrescaler(1);
	Timer3::setOverflow(1200);
	Timer3::configureOutputChannel(1, Timer3::OUTPUT_PWM, 0);
	Timer3::configureOutputChannel(2, Timer3::OUTPUT_PWM, 0);
	Timer3::configureOutputChannel(3, Timer3::OUTPUT_PWM, 0);
	Timer3::configureOutputChannel(4, Timer3::OUTPUT_PWM, 0);
	Timer3::applyAndReset();
	Timer3::start();

	if(!M1_FS::read()){
		//fault state
		State::set(State::MOTOR1_FAULT);
	}

	if(!M2_FS::read()){
		//fault state
		State::set(State::MOTOR2_FAULT);
	}
	return true;
}


template<>
bool Motor<X>::getFaultStatus(){
	return M1_FS::read();
}

template<>
bool Motor<Y>::getFaultStatus(){
	return M2_FS::read();
}

/**
 * Motor 1 fault state handler
 * Connected to PB2
 */
extern "C" void
EXTI2_IRQHandler(void) {
	ExtInt::resetInterruptFlag(ExtInt::Pin::PB2);
	if(!M1_FS::read()){
		//fault state
		State::set(State::MOTOR1_FAULT);
	}
}

/**
 * Motor 2 fault state handler
 * Connected to PB4
 */
extern "C" void
EXTI4_IRQHandler(void) {
	ExtInt::resetInterruptFlag(ExtInt::Pin::PB4);
	if(!M2_FS::read()){
		//fault state
		State::set(State::MOTOR2_FAULT);
	}
}
