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

#ifndef XPCC_LPC17XX__GPIO_HPP
#define XPCC_LPC17XX__GPIO_HPP

#include <xpcc/architecture/driver/gpio.hpp>

/*
 * 32-bit ports.
 * 
 * Each port pin can be programmed to generate an interrupt on a rising edge,
 * a falling edge, or both.
 * Edge detection is asynchronous, so it may operate when clocks are not
 * present, such as during Power-down mode. With this feature, level triggered
 * interrupts are not needed.
 * 
 * GPIO0 and GPIO2 interrupts share the same position in the NVIC with External
 * Interrupt 3.
 * 
 */
namespace xpcc
{
	namespace lpc
	{
		/**
		 * \ingroup	lpc17xx
		 */
		enum InputType
		{
			FLOATING = (0 << 3),
			PULLUP   = (1 << 3),
			PULLDOWN = (2 << 3),
			REPEATER = (3 << 3),
		};
		
		/**
		 * \ingroup	lpc17xx
		 */
		enum OutputType
		{
			PUSH_PULL  = (0 << 10),
			OPEN_DRAIN = (1 << 10),
		};
	}
}

/**
 * \ingroup	lpc17xx
 * \brief	Create a input/output pin type
 * 
 * \hideinitializer
 */
#define	GPIO__IO(name, port, pin) \
	struct name { \
		ALWAYS_INLINE static void \
		setOutput(bool status) { \
			set(status); \
			setOutput(); } \
		ALWAYS_INLINE static void \
		setOutput() { \
				CONCAT(LPC_GPIO, port)->FIODIR |= (1 << pin);\
			} \
		ALWAYS_INLINE static void \
		setInput() { \
			CONCAT(LPC_GPIO, port)->FIODIR &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static void \
		set() { CONCAT(LPC_GPIO, port)->FIOSET = (1 << pin); } \
		ALWAYS_INLINE static void \
		reset() { CONCAT(LPC_GPIO, port)->FIOCLR  = (1 << pin); } \
		ALWAYS_INLINE static void \
		toggle() { if (read()) { reset(); } else { set(); } } \
		ALWAYS_INLINE static void \
		set(bool status) { if (status) { set(); } else { reset(); } } \
		ALWAYS_INLINE static bool \
		read() { return (CONCAT(LPC_GPIO, port)->FIOPIN & (1 << pin)); } \
	}

/**
 * \brief	Create a output pin type
 * 
 * Examples:
 * \code
 * GPIO__OUTPUT(Led, 0, 7);
 * 
 * Led::setOutput();
 * Led::setOutput(xpcc::lpc::PUSH_PULL);
 * Led::setOutput(xpcc::lpc::OPEN_DRAIN);
 * 
 * Led::set();
 * Led::reset();
 * 
 * Led::toggle();
 * \endcode
 * 
 * \hideinitializer
 * \ingroup	lpc11xx
 */
#define	GPIO__OUTPUT(name, port, pin) \
	struct name { \
		ALWAYS_INLINE static void setOutput(bool status) { \
			set(status); \
			setOutput(); } \
		ALWAYS_INLINE static void \
		setOutput() { \
			CONCAT(LPC_GPIO, port)->FIODIR |= (1 << pin);\
		} \
		ALWAYS_INLINE static void \
		set() { CONCAT(LPC_GPIO, port)->FIOSET = (1 << pin); } \
		ALWAYS_INLINE static void \
		reset() { CONCAT(LPC_GPIO, port)->FIOCLR  = (1 << pin); } \
		ALWAYS_INLINE static void \
		toggle() { if (read()) { reset(); } else { set(); } } \
		ALWAYS_INLINE static void \
		set(bool status) { if (status) { set(); } else { reset(); } } \
	protected: \
		ALWAYS_INLINE static bool \
		read() { return (CONCAT(LPC_GPIO, port)->FIOPIN & (1 << pin)); } \
	}

/**
 * \brief	Create a input type
 * 
 * Examples:
 * \code
 * GPIO__INPUT(Button, 0, 3);
 * 
 * Button::setInput();
 * Button::setInput(xpcc::lpc::PULLUP);
 * Button::setInput(xpcc::lpc::PULLDOWN);
 * 
 * if (Button::read()) {
 *     ...
 * }
 * \endcode
 * 
 * \hideinitializer
 * \ingroup	lpc11xx
 */
#define GPIO__INPUT(name, port, pin) \
	struct name { \
		ALWAYS_INLINE static void \
		setInput() { \
			CONCAT(LPC_GPIO, port)->FIODIR &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static bool \
		read() { return (CONCAT(LPC_GPIO, port)->FIOPIN & (1 << pin)); } \
	}

#endif // XPCC_LPC17XX__GPIO_HPP
