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

#ifndef XPCC_ATXMEGA__TIMER_AWEX_C_HPP
#define XPCC_ATXMEGA__TIMER_AWEX_C_HPP

#include <avr/io.h>
#include <stdint.h>

#if defined(AWEXC) || defined(__DOXYGEN__)

namespace xpcc
{
	namespace atxmega
	{
		/**
		 * \brief		Advanced Waveform EXtension of Timer C
		 *
		 * \ingroup		atxmega_timer
		 */
		class WaveformC
		{
		public:
			inline static AWEX_t&
			getWaveformBase()
			{
				return AWEXC;
			}
			
			inline static void
			setAWEXMode(uint8_t mode)
			{
				AWEXC_CTRL = (AWEXC_CTRL & ~(AWEX_PGM_bm|AWEX_CWCM_bm)) | mode;
			}
			
			inline static void
			setAWEXDTIEnable(uint8_t selection)
			{
				AWEXC_CTRL = (AWEXC_CTRL & ~(AWEX_DTICCDEN_bm|AWEX_DTICCCEN_bm|AWEX_DTICCBEN_bm|AWEX_DTICCAEN_bm)) | selection;
			}
			
			inline static void
			setAWEXFaultDetection(uint8_t mode)
			{
				AWEXC_FDCTRL = mode;
			}
			
			inline static uint8_t
			getAWEXStatus()
			{
				return AWEXC_STATUS;
			}
		};
	}
}

#endif	// AWEXC
#endif // XPCC_ATXMEGA__TIMER_AWEX_C_HPP
