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

#ifndef XPCC_I2C__MASTER_HPP
#define XPCC_I2C__MASTER_HPP

#include <cstddef>
#include <stdint.h>

#include <xpcc/architecture/utils.hpp>
#include <xpcc/driver/interface.hpp>
#include "constants.hpp"
#include "delegate.hpp"

namespace xpcc
{
	/**
	 * \brief	I2C Interface
	 * \ingroup	i2c
	 */
	namespace i2c
	{
		/**
		 * \brief	Interface of a I2C master
		 * 
		 * Performing transfers in background allows the use of DMA etc. and
		 * enables the program to do other things while the operation is
		 * going on.
		 *
		 * \author Georgi Grinshpun
		 * \author Niklas Hauser
		 * \ingroup	interface
		 */
		class Master : public Interface
		{
		public:
			/**
			 * \brief	Requests bus control and starts the transfer.
			 *
			 * \param	delegate	object that inherits from the Delegate class.
			 * \return	Caller gains control if \c true. Call has no effect if \c false.
			 */
			bool
			start(Delegate *delegate);
			
			/**
			 * \brief	Requests bus control and starts the transfer.
			 *			Blocks until delegate is detached.
			 *
			 * \param	delegate	object that inherits from the Delegate class.
			 * \return	Caller gains control if \c true. Call has no effect if \c false.
			 */
			bool
			startSync(Delegate *delegate);
			
			/**
			 * \brief	Perform a software reset of the driver.
			 *	
			 * This method calls the stopped Delegate method and then detaches 
			 * the delegate.
			 * \param error DetachCause \c ERROR_CONDITION if \c true, else \c SOFTWARE_RESET
			 */
			void
			reset(bool error=false);
			
#ifdef __DOXYGEN__
			/**
			 * \brief	Check the error state of the driver.
			 *	
			 * Since the error states are hardware and implementation specific,
			 * this is only the recommended interface and does not need to be implemented.
			 */
			uint8_t
			getErrorState();
#endif
		};
	}
}

#endif // XPCC_I2C__MASTER_HPP
