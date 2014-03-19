// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2012, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------

/**
 * \ingroup		atxmega
 * \defgroup	atxmega_adc	Analog/Digital Converter Module
 * 
 * The xMEGA family contains one (A4 family) or two (A1,A3 family) 12-bit
 * SAR (Successive Approximation Result) ADCs connected to PORTA and PORTB. 
 * 
 * ADC clock frequency should be at around 62kHz for maximum
 * resolution when measuring internal signals. If less than 12 bits are needed
 * the frequency can be higher.
 * 
 * For external signals the frequency should be between 100kHz and 2MHz.
 * 
 * A differential input in signed mode increases accuracy by up to 6 bit (!).
 * \see	<a href="http://blog.frankvh.com/2010/01/03/atmel-xmega-adc-problems-solutions/">
 * XMEGA ADC Problems and Solutions</a>
 *
 * Also read <a href=" http://www.bostonandroid.com/manuals/xmega-precision-adc-howto.html/">
 * this blogpost</a>.
 * 
 * The XMEGA ADC is more complex than of the Mega ADC, and each ADC Module has
 * four ADC Channels which have a separate class. You first have to initialize
 * the ADC Module to use any of the Channels.
 * 
 * Select either a single ended or differential input mode. Check the
 * conversion mode bit in the ADC Module for a signed conversion.
 * Then select the negative and/or positive inputs for the channel.
 *
 * This example configures the ADC Module only.
 * \code
 * #include <xpcc/architecture/platform/atxmega/adc.hpp>
 *
 * int
 * main(void)
 * {
 *     ...
 *     AdcModuleA::initialize(xpcc::atxmega::adc::REFERENCE_EXTERNAL_PORTA);
 *     AdcModuleA::setSignedConversion(true);
 *     ...
 * }
 * \endcode
 *
 * The Channels also inherit from their Module.
 * Here the module is configured by the channel. This is recommended way.
 * \code
 * #include <xpcc/architecture/platform/atxmega/adc.hpp>
 * 
 * int
 * main(void)
 * {
 *     ...
 *     typedef AdcChannelA0 myChannel;
 *     
 *     myChannel::initialize(xpcc::atxmega::adc::REFERENCE_EXTERNAL_PORTA);    // PortA AREF
 *     
 *     // Differential input mode
 *     myChannel::setDifferentialInput(xpcc::atxmega::adc::POSITIVE_PIN4, xpcc::atxmega::adc::NEGATIVE_PIN3);
 *     
 *     // The result is always unsigned, so you want to cast it in signed mode
 *     int16_t result = static_cast<int16_t>(myChannel::read());
 *     ...
 * }
 * \endcode
 *
 * \author Niklas Hauser
 */

#include "adc/adca.hpp"
#include "adc/adcb.hpp"
#include "adc/adca_channel0.hpp"
#include "adc/adca_channel1.hpp"
#include "adc/adca_channel2.hpp"
#include "adc/adca_channel3.hpp"
#include "adc/adcb_channel0.hpp"
#include "adc/adcb_channel1.hpp"
#include "adc/adcb_channel2.hpp"
#include "adc/adcb_channel3.hpp"
