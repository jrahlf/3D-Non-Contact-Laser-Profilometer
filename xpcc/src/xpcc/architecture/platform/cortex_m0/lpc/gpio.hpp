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

#ifndef XPCC_LPC11XX__GPIO_HPP
#define XPCC_LPC11XX__GPIO_HPP

#include <xpcc/architecture/driver/gpio.hpp>

// All these pins are not GPIOs as default.
// Some special handling is necessary.
// To use them as GPIO the LPC_IOCON register must be set.
#define PIO0_0		RESET_PIO0_0
#define PIO0_10		SWCLK_PIO0_10
#define PIO0_11		R_PIO0_11

#define PIO1_0		R_PIO1_0
#define PIO1_1		R_PIO1_1
#define PIO1_2		R_PIO1_2
#define PIO1_3		SWDIO_PIO1_3


#define IOCON_ADMODE_DIGITAL (1<<7)	// Set Pin to Digital Mode

/*
 * 12-bit ports.
 */
namespace xpcc
{
	namespace lpc
	{
		/**
		 * \ingroup	lpc11xx
		 */
		enum InputType
		{
			FLOATING = (0 << 3),
			PULLDOWN = (1 << 3),
			PULLUP   = (2 << 3),
			REPEATER = (3 << 3),
		};
		
		/**
		 * \ingroup	lpc11xx
		 */
		enum OutputType
		{
			PUSH_PULL  = (0 << 10),
			OPEN_DRAIN = (1 << 10),
		};
	}
}

/**
 * \ingroup	lpc11xx
 * \brief	Create a input/output pin type
 * 
 * \hideinitializer
 */
#define	GPIO__IO(name, port, pin) \
	struct name { \
		ALWAYS_INLINE static void \
		setOutput(bool status) { \
			setOutput(); \
			set(status); \
		} \
		ALWAYS_INLINE static void \
		setOutput(::xpcc::lpc::OutputType type = ::xpcc::lpc::PUSH_PULL) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) = type | IOCON_ADMODE_DIGITAL; \
			CONCAT(LPC_GPIO, port)->DIR |= 1 << pin; \
		} \
		ALWAYS_INLINE static void \
		setInput(::xpcc::lpc::InputType type = ::xpcc::lpc::FLOATING) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin)  =       type | IOCON_ADMODE_DIGITAL; \
			CONCAT(LPC_GPIO, port)->DIR           &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static void set()    { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = (1 << pin); } \
		ALWAYS_INLINE static void reset()  { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = 0;          } \
		ALWAYS_INLINE static void toggle()         { if (read()) { reset(); } else {   set(); } } \
		ALWAYS_INLINE static void set(bool status) { if (status) {   set(); } else { reset(); } } \
		ALWAYS_INLINE static bool read()           { return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; } \
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
		ALWAYS_INLINE static void \
		setOutput(bool status) { \
			setOutput(); \
			set(status); \
		} \
		ALWAYS_INLINE static void \
		setOutput(::xpcc::lpc::OutputType type = ::xpcc::lpc::PUSH_PULL) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin) = type | IOCON_ADMODE_DIGITAL; \
			CONCAT(LPC_GPIO, port)->DIR |= 1 << pin; \
		} \
		ALWAYS_INLINE static void set()            { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = (1 << pin); } \
		ALWAYS_INLINE static void reset()          { CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin] = 0;          } \
		ALWAYS_INLINE static void toggle()         { if (read()) { reset(); } else {   set(); } } \
		ALWAYS_INLINE static void set(bool status) { if (status) {   set(); } else { reset(); } } \
	protected: \
		ALWAYS_INLINE static bool \
		read() { \
			return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; \
		} \
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
		setInput(::xpcc::lpc::InputType type = ::xpcc::lpc::FLOATING) { \
			LPC_IOCON->CONCAT4(PIO, port, _, pin)  =       type | IOCON_ADMODE_DIGITAL; \
			CONCAT(LPC_GPIO, port)->DIR           &= ~(1 << pin); \
		} \
		ALWAYS_INLINE static bool \
		read() { \
			return (CONCAT(LPC_GPIO, port)->MASKED_ACCESS[1 << pin]) >> pin; \
		} \
	}


namespace xpcc
{
	/**
	 * \brief	General purpose input and/or output pins
	 * \ingroup	gpio
	 */
	namespace lpc
	{
	template <typename Pin>
	class Invert
	{
	public:
		ALWAYS_INLINE static void
		setOutput()
		{
			Pin::setOutput();
		}

		ALWAYS_INLINE static void
		setOutput(bool value)
		{
			Pin::setOutput(!value);
		}

		ALWAYS_INLINE static void
		setInput()
		{
			Pin::setInput();
		}

		ALWAYS_INLINE static void
		setInput(xpcc::lpc::InputType t)
		{
			Pin::setInput(t);
		}

		ALWAYS_INLINE static void
		set()
		{
			Pin::reset();
		}

		ALWAYS_INLINE static void
		set(bool value)
		{
			Pin::set(!value);
		}

		ALWAYS_INLINE static void
		reset()
		{
			Pin::set();
		}

		ALWAYS_INLINE static void
		toggle()
		{
			Pin::toggle();
		}

		ALWAYS_INLINE static bool
		read()
		{
			return !Pin::read();
		}
	};

	} // gpio namespace
} // xpcc namespace

#endif // XPCC_LPC11XX__GPIO_HPP
