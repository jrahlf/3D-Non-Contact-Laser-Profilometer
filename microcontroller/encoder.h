/*
 * encoder.h
 *
 *  Created on: Nov 26, 2013
 *      Author: jonas
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <xpcc/architecture.hpp>

namespace Encoder{

/**
 * Initializes Timer 2 and Timer 5 to be used to decode quadrature
 * encoder signal in 4x decoding
 * @return
 */
bool init();

/**
 * Zeros the internal encoder values
 */
void zero();

/**
 *
 */
void pause();

void start();

};


#endif /* ENCODER_H_ */
