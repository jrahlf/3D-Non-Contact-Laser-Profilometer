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

#include <stdint.h>

#include <xpcc/debug/logger.hpp>
#include <xpcc_config.hpp>

// ----------------------------------------------------------------------------
extern "C"
void
_hardFaultHandler(const uint32_t * ctx)
{
#if CORTEX_ENABLE_HARD_FAULT_HANDLER != 0
#undef XPCC_LOG_LEVEL
#define XPCC_LOG_LEVEL	xpcc::log::ERROR
	
	uint32_t stacked_r0 = ((uint32_t) ctx[0]);
	uint32_t stacked_r1 = ((uint32_t) ctx[1]);
	uint32_t stacked_r2 = ((uint32_t) ctx[2]);
	uint32_t stacked_r3 = ((uint32_t) ctx[3]);
	
	uint32_t stacked_r12 = ((uint32_t) ctx[4]);
	uint32_t stacked_lr  = ((uint32_t) ctx[5]);
	uint32_t stacked_pc  = ((uint32_t) ctx[6]);
	uint32_t stacked_psr = ((uint32_t) ctx[7]);
	
	uint32_t bfar = (*((volatile uint32_t *)(0xE000ED38)));
	uint32_t cfsr = (*((volatile uint32_t *)(0xE000ED28)));
	uint32_t hfsr = (*((volatile uint32_t *)(0xE000ED2C)));
	uint32_t dfsr = (*((volatile uint32_t *)(0xE000ED30)));
	uint32_t afsr = (*((volatile uint32_t *)(0xE000ED3C)));
	
	XPCC_LOG_ERROR.printf("\n\nHard fault Exception:\n");
	XPCC_LOG_ERROR.printf("r0 : 0x%08lx   r12 : 0x%08lx\n", stacked_r0, stacked_r12);
	XPCC_LOG_ERROR.printf("r1 : 0x%08lx   lr  : 0x%08lx\n", stacked_r1, stacked_lr);
	XPCC_LOG_ERROR.printf("r2 : 0x%08lx   pc  : 0x%08lx\n", stacked_r2, stacked_pc);
	XPCC_LOG_ERROR.printf("r3 : 0x%08lx   psr : 0x%08lx\n", stacked_r3, stacked_psr);
	XPCC_LOG_ERROR.printf("BFAR : 0x%08lx\n", bfar);
	XPCC_LOG_ERROR.printf("CFSR : 0x%08lx\n", cfsr);
	XPCC_LOG_ERROR.printf("HFSR : 0x%08lx\n", hfsr);
	XPCC_LOG_ERROR.printf("DFSR : 0x%08lx\n", dfsr);
	XPCC_LOG_ERROR.printf("AFSR : 0x%08lx\n", afsr);
#else
	(void) ctx;	// avoid warning
#endif
	
	// Infinite loop
	while (1)
	{
	}
}

