// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
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
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_ATXMEGA__TIMER_INTERRUPT_C0_HPP
#define XPCC_ATXMEGA__TIMER_INTERRUPT_C0_HPP

#include <avr/io.h>
#include <stdint.h>
#include <xpcc/utils/dummy.hpp>

#include "timer.hpp"
#include "timer_c0.hpp"

#if defined(TCC0) || defined(__DOXYGEN__)

namespace xpcc
{
	namespace atxmega
	{
		/**
		 * \brief		Timer Interrupt Module C0
		 *
		 * \ingroup		atxmega_timer
		 */
		class TimerInterruptC0 : public TimerC0
		{
		public:
			typedef void (*F) ();
			
			inline static void
			attachErrorInterrupt(TC_ERRINTLVL_t level, F function=xpcc::dummy)
			{
				setErrorInterrupt(level);
				error = function;
			}
			
			inline static void
			attachOverflowInterrupt(TC_OVFINTLVL_t level, F function=xpcc::dummy)
			{
				setOverflowInterrupt(level);
				overflow = function;
			}
			
			inline static void
			attachCompareCaptureAInterrupt(TC_CCAINTLVL_t level, F function=xpcc::dummy)
			{
				setCompareCaptureAInterrupt(level);
				cca = function;
			}
			
			inline static void
			attachCompareCaptureBInterrupt(TC_CCBINTLVL_t level, F function=xpcc::dummy)
			{
				setCompareCaptureBInterrupt(level);
				ccb = function;
			}
			inline static void
			attachCompareCaptureCInterrupt(TC_CCCINTLVL_t level, F function=xpcc::dummy)
			{
				setCompareCaptureCInterrupt(level);
				ccc = function;
			}
			
			inline static void
			attachCompareCaptureDInterrupt(TC_CCDINTLVL_t level, F function=xpcc::dummy)
			{
				setCompareCaptureDInterrupt(level);
				ccd = function;
			}
			
			/**
			 * Checks the Channel and calls the suitable function.
			 * Make sure to use the interrupt level from Channel A!
			 * \return \c false if you try to access a non-existant Timer Channel
			 */
			static bool
			attachCompareCaptureInterrupt(xpcc::timer::Channel channel, uint8_t level, F function=xpcc::dummy);
			
			// specific configuration combinations
			/**
			 * \brief Enable OVL interrupt in x ms intervals
			 * 
			 * Calls the function in the interrupt independently.
			 * 
			 * \param function the function to call every x ms
			 * \param interval between interrupts in ms
			 */
			static void
			setMsTimer(F function=xpcc::dummy, uint8_t interval=1);
			
		public:
			static F overflow;
			static F error;
			static F cca;
			static F ccb;
			static F ccc;
			static F ccd;
		};
	}
}

#endif // TCC0
#endif // XPCC_ATXMEGA__TIMER_INTERRUPT_C0_HPP