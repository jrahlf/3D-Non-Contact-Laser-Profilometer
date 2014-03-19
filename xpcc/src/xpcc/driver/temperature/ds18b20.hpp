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

#ifndef XPCC__DS18B20_HPP
#define XPCC__DS18B20_HPP

#include <xpcc/driver/connectivity/one_wire.hpp>

namespace xpcc
{
	/**
	 * \brief	Programmable resolution 1-wire digital thermometer
	 * 
	 * The DS18B20 digital thermometer provides 9-bit to 12-bit Celsius
	 * temperature measurements and has an alarm function with non-volatile
	 * user-programmable upper and lower trigger points. The DS18B20
	 * communicates over a 1-Wire bus that by definition requires only one
	 * data line (and ground) for communication with a central microprocessor.
	 * 
	 * It has an operating temperature range of -55°C to +125°C and is
	 * accurate to ±0.5°C over the range of -10°C to +85°C.
	 * 
	 * In addition, the DS18B20 can derive power directly from the data line
	 * ("parasite power"), eliminating the need for an external power supply.
	 * 
	 * Features:
	 * - Power Supply Range is 3.0V to 5.5V 
	 * - Each device has a unique 64-Bit Serial Code Stored in an On-Board ROM
	 * - Thermometer Resolution is User Selectable from 9 to 12 Bits
	 * - Converts Temperature to 12-Bit Digital Word in 750ms (Max)
	 * 
	 * \todo	Implement the configure() method to set the resolution
	 * 			and the temperature alarm functionality
	 * 
	 * \author	Fabian Greif
	 * \ingroup	temperature
	 */
	template <typename OneWire>
	class Ds18b20
	{
	public:
		/**
		 * \brief	Constructor
		 * 
		 * Default resolution is 12-bit.
		 * 
		 * \param 	rom		8-byte unique ROM number of the device
		 */
		Ds18b20(const uint8_t *rom);
		
		// TODO
		//void
		//configure();
		
		/**
		 * \brief	Check if the device is present
		 * 
		 * \return	\c true if the device is found, \c false if the
		 * 			ROM number is not available on the bus.
		 */
		bool
		isAvailable();
		
		/**
		 * \brief	Start the conversion of this device
		 */
		void
		startConversion();
		
		/**
		 * \brief	Start the conversion for all connected devices
		 * 
		 * Uses the SKIP_ROM command to start the conversion on all
		 * connected DS18B20 devices.
		 * 
		 * \warning	Use this function with caution. If you have devices other
		 * 			than the DS18B20 connected to your 1-wire bus check if
		 * 			they tolerate the SKIP_ROM + CONVERT_T command.
		 */
		void
		startConversions();
		
		/**
		 * \brief	Check if the last conversion is complete
		 * 
		 * \return	\c true conversion complete, \n
		 * 			\c false conversion in progress.
		 */
		bool
		isConversionDone();
		
		/**
		 * \brief	Read the current temperature in centi-degree
		 * 
		 * \todo	Needs a better output format
		 * \return	temperature in centi-degree
		 */
		int16_t
		readTemperature();
		
	protected:
		/**
		 * \brief	Select the current device
		 */
		bool
		selectDevice();
		
		uint8_t identifier[8];
		
		static const uint8_t CONVERT_T = 0x44;
		static const uint8_t WRITE_SCRATCHPAD = 0x4e;
		static const uint8_t READ_SCRATCHPAD = 0xbe;
		static const uint8_t COPY_SCRATCHPAD = 0x48;
		static const uint8_t RECALL_E2 = 0xb8;
		static const uint8_t READ_POWER_SUPPLY = 0xb4;
		
		static OneWire ow;
	};
}

#include "ds18b20_impl.hpp"

#endif // XPCC__DS18B20_HPP
