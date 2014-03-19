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

#ifndef XPCC__SOFTWARE_I2C_HPP
#define XPCC__SOFTWARE_I2C_HPP

#include <xpcc/architecture/driver/delay.hpp>
#include "master.hpp"

namespace xpcc
{
	/**
	 * \brief	Software emulation of a I2C master implementation
	 * 
	 * \tparam	Scl			any IO-Pin (see GPIO__IO())
	 * \tparam	Sda			any IO-Pin (see GPIO__IO())
	 * \tparam	Frequency	in Hz (default frequency is 100kHz)
	 * 
	 * \ingroup	i2c
	 * \see		gpio
	 */
	template< typename Scl,
			  typename Sda,
			  int32_t Frequency = 100000 >
	class SoftwareI2C : public i2c::Master
	{
	public:
		enum ErrorState
		{
			NO_ERROR,			//!< No Error occurred
			DATA_NACK,			//!< Data was transmitted and NACK received
			ARBITRATION_LOST,	//!< Arbitration was lost during writing or reading
			BUS_ERROR,			//!< Misplaced Start or Stop condition
			UNKNOWN_ERROR		//!< Unknown error condition
		};
		
		/**
		 * \brief	Initialize the hardware
		 */
		static void
		initialize(bool pullup=false);
		
	public:
		static void
		reset(bool error=false);
		
		static bool
		start(xpcc::i2c::Delegate *delegate);
		
		static bool
		startSync(xpcc::i2c::Delegate *delegate)
		{
			return start(delegate);
		};
		
		static uint8_t
		getErrorState();

	private:
		static void
		error();
		
	private:
		static void
		startCondition();
		
		static void
		stopCondition();
		
		static bool
		write(uint8_t data);
		
		static uint8_t
		read(bool ack);

	private:
		static inline bool
		readBit();
		
		static inline void
		writeBit(bool bit);
		
		static inline void
		sclSetAndWait();

		static inline void
		delay();
		
		// calculate the delay in microseconds needed to achieve the
		// requested SPI frequency
		static constexpr float delayTime = (1000000.0 / Frequency) / 2.0;
		
		static Scl scl;
		static Sda sda;
		
		static xpcc::i2c::Delegate::NextOperation nextOperation;
		static xpcc::i2c::Delegate *myDelegate;
		static uint8_t errorState;
	};
}

#include "software_i2c_impl.hpp"

#endif // XPCC__SOFTWARE_I2C_HPP
