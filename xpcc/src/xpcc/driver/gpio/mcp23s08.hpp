// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
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

#ifndef XPCC__MCP23S08_HPP
#define XPCC__MCP23S08_HPP

#include <xpcc/architecture/utils.hpp>
#include <xpcc/architecture/driver/gpio.hpp>

namespace xpcc
{
	/**
	 * \brief	8-Bit I/O Expander with Serial Interface
	 * 
	 * A1 and A0 need to be tided low.
	 * 
	 * \author	Fabian Greif
	 * \ingroup	driver_gpio
	 */
	template <typename Spi, typename Cs, typename Int>
	class Mcp23s08
	{
	public:
		static void
		initialize();
		
		/**
		 * @brief	Configure pins
		 * 
		 * @param	inputMask	1=Input, 0=Output
		 * @param	pullupMask	1=Pullup, 0=Pullup-Disabled
		 */
		static void
		configure(uint8_t inputMask, uint8_t pullupMask);
		
		//void
		//configureInterrupt();
		
		static uint8_t
		read();
		
		static void
		write(uint8_t output);
		
	protected:
		enum RegisterAddress
		{
			MCP_IODIR = 0x00,		///< Port direction (1=input, 0=output)
			MCP_IPOL = 0x01,		///< Invert polarity
			MCP_GPINTEN = 0x02,		///< Enable interrupt
			MCP_DEFVAL = 0x03,		///< Compare register for interrupt
			MCP_INTCON = 0x04,
			MCP_IOCON = 0x05,		///< Configuration
			MCP_GPPU = 0x06,		///< Enable pullups
			MCP_INTF = 0x07,		///< Interrupt flag register
			MCP_INTCAP = 0x08,		///< Interrupt capture register
			MCP_GPIO = 0x09,		///< Port values
			MCP_OLAT = 0x0a			///< Output latch register
		};
		
		enum RW
		{
			WRITE = 0,
			READ = 1
		};
		
		static const uint8_t deviceAddress = 0x40;
		
		static Spi spi;
		static Cs cs;
		static Int interrupt;
	};
}

#include "mcp23s08_impl.hpp"

#endif	// XPCC__MCP23S08_HPP
