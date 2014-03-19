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

#ifndef XPCC_ATMEGA__TWI_MASTER_HPP
#define XPCC_ATMEGA__TWI_MASTER_HPP

#include <xpcc/driver/connectivity/i2c/master.hpp>
#include <stdint.h>

namespace xpcc
{
	namespace atmega
	{
		/**
		 * \brief	Interrupt-driven I2C master module.
		 *
		 * Interrupts must be enabled.
		 * 
		 * \author Niklas Hauser
		 * \ingroup	atmega
		 * \ingroup	i2c
		 */
		class I2cMaster : public ::xpcc::i2c::Master
		{
		public:
			enum ErrorState
			{
				NO_ERROR,			//!< No Error occurred
				DATA_NACK,			//!< Data was transmitted and NACK received
				ADDRESS_NACK,		//!< Address was transmitted and NACK received
				ARBITRATION_LOST,	//!< Arbitration was lost during writing or reading
				BUS_ERROR,			//!< Misplaced Start or Stop condition
				UNKNOWN_ERROR		//!< Unknown error condition
			};
			/**
			 * \brief	Initialize hardware
			 * 
			 * Sets the SCL frequency. The CPU clock frequency in the slave must
			 * be at least 16 times higher than the SCL frequency. Note that
			 * slaves may prolong the SCL low period, thereby reducing the average
			 * TWI bus clock period. The SCL frequency is generated according to
			 * the following equation:
			 * 
			 * \code
			 * f_scl = f_cpu / (16 + 2 * twbr * 4 ^ twps)
			 *       = f_cpu / (16 + 2 * twbr * prescaler)
			 * 
			 * twbr = (f_cpu / f_scl - 16) / (2 * prescaler)
			 * \endcode
			 * 
			 * with: \n
			 * \c twbr in range 0..255 \n
			 * \c twps in range 0..3
			 * 
			 * \code
			 *  twps | prescaler
			 * ------+-----------
			 *   0   |     1
			 *   1   |     4
			 *   2   |    16
			 *   3   |    64
			 * \endcode
			 * 
			 * Examples:
			 * \code
			 *   f_cpu  | f_scl |      settings
			 *   [MHz]  | [kHz] |
			 * ---------+-------+----------------------
			 *  16.0000 |  400  | twbr =  12, twps = 0
			 *          |  100  | twbr =  72, twps = 0
			 *          |   10  | twbr = 198, twps = 1
			 *  14.7456 |  400  | twbr =  10, twps = 0
			 *          |  100  | twbr =  65, twps = 0
			 *          |   10  | twbr = 182, twps = 1
			 *   8.0000 |  400  | twbr =   2, twps = 0
			 *          |  100  | twbr =  32, twps = 0
			 *          |   10  | twbr =  98, twps = 1
			 * \endcode
			 */
			static void
			initialize(uint8_t twbr, uint8_t twps);
			
			static void
			reset(bool error=false);
			
		public:
			static bool
			start(xpcc::i2c::Delegate *delegate);
			
			static bool
			startSync(xpcc::i2c::Delegate *delegate);
			
			/**
			 * This method returns the actual error state if it is called
			 * from the delegates stop method.
			 */
			static uint8_t
			getErrorState();
		};
	}
}

#endif // XPCC_ATMEGA__TWI_MASTER_HPP
