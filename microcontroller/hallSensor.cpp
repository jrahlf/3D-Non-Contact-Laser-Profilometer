/*
 * hallSensor.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: jonas
 */

#include "hallSensor.h"
#include <xpcc/architecture.hpp>

//#pragma GCC push_options
//#pragma GCC optimize ("O0")

GPIO__INPUT(XFRONT, E, 11);
GPIO__INPUT(XEND, E, 7);
GPIO__INPUT(YFRONT, D, 8);
GPIO__INPUT(YEND, D, 9);


using namespace xpcc::stm32;

template<>
bool HallSensor<ALL>::init(){
	for(int i = 0; i < 2; i++){
		triggered[i] = false;
		//state array will be initialized with real values
	}
	XFRONT::setInput(xpcc::stm32::PULLUP);
	XEND::setInput(xpcc::stm32::PULLUP);
	YFRONT::setInput(xpcc::stm32::PULLUP);
	YEND::setInput(xpcc::stm32::PULLUP);
	return true;

	//xpcc::delay_ms(50); //important to prevent race condition!!
#if 1
	//should not be necessary but still..
	/*HallSensor<X>::setFrontState(XFRONT::read());
	HallSensor<X>::setEndState(XEND::read());
	HallSensor<Y>::setFrontState(YFRONT::read());
	HallSensor<Y>::setEndState(YEND::read());*/

	ExtInt::enable(ExtInt::Pin::PE11);
	ExtInt::setMode(ExtInt::Pin::PE11, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PE11);
	ExtInt::enableInterruptVector(ExtInt::Pin::PE11, 6);

	ExtInt::enable(ExtInt::Pin::PE7);
	ExtInt::setMode(ExtInt::Pin::PE7, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PE7);
	ExtInt::enableInterruptVector(ExtInt::Pin::PE7, 6);

	ExtInt::enable(ExtInt::Pin::PD8);
	ExtInt::setMode(ExtInt::Pin::PD8, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PD8);
	ExtInt::enableInterruptVector(ExtInt::Pin::PD8, 6);

	ExtInt::enable(ExtInt::Pin::PD9);
	ExtInt::setMode(ExtInt::Pin::PD9, ExtInt::Mode::RisingFalling);
	ExtInt::enableInterrupt(ExtInt::Pin::PD9);
	ExtInt::enableInterruptVector(ExtInt::Pin::PD9, 6);
	return true;
}

extern "C" void
EXTI11_IRQHandler(void) {
	if (xpcc::stm32::ExtInt::getInterruptFlag(ExtInt::Pin::PE11)) {
		ExtInt::resetInterruptFlag(ExtInt::Pin::PE11);
		//HallSensor<X>::setFrontState(XFRONT::read());
		//State::setBlue(!XFRONT::read());
	}
	Motor<X>::setSpeed(0);
	Motor<Y>::setSpeed(0);

}

extern "C" void
EXTI9_5_IRQHandler(void) {
	if (xpcc::stm32::ExtInt::getInterruptFlag(ExtInt::Pin::PE7)) {
		ExtInt::resetInterruptFlag(ExtInt::Pin::PE7);
		//HallSensor<X>::setEndState(XEND::read());
		//State::setRed(!XEND::read());
	}
	else if (xpcc::stm32::ExtInt::getInterruptFlag(ExtInt::Pin::PD8)) {
		ExtInt::resetInterruptFlag(ExtInt::Pin::PD8);
		//HallSensor<Y>::setFrontState(YFRONT::read());
		//State::setBlue(!YFRONT::read());
	}
	else if (xpcc::stm32::ExtInt::getInterruptFlag(ExtInt::Pin::PD9)) {
		ExtInt::resetInterruptFlag(ExtInt::Pin::PD9);
		//HallSensor<Y>::setEndState(YEND::read());
		//State::setRed(!YEND::read());
	}

	Motor<X>::setSpeed(0);
	Motor<Y>::setSpeed(0);

}

//#pragma GCC pop_options

#else
}
#endif

