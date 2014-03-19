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

#include <xpcc/architecture/platform/cortex_m3/stm32.hpp>
#include "clock.hpp"

// ----------------------------------------------------------------------------
bool
xpcc::stm32::Clock::enableHse(HseConfig config, uint32_t waitCycles)
{
	if (config == HseConfig::HSE_BYPASS) {
		RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
	}
	else {
		RCC->CR |= RCC_CR_HSEON;
	}

	uint32_t t = waitCycles;
	while (!(RCC->CR & RCC_CR_HSERDY) && --t) {
	}

	return (RCC->CR & RCC_CR_HSERDY);
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::Clock::enablePll(PllSource source, uint8_t pllM,
		uint16_t pllN)
{
	uint32_t tmp = 0;
	
	// Read reserved values and clear all other values
	tmp |= RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM
			| RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ);
	
	// PLLSRC source for pll and for plli2s
	tmp |= (source == PllSource::PLL_HSI) ? RCC_PLLCFGR_PLLSRC_HSI
			: RCC_PLLCFGR_PLLSRC_HSE;
	
	// PLLM (0) = factor is user defined VCO input frequency must be configured to 2MHz
	tmp |= ((uint32_t) pllM) & RCC_PLLCFGR_PLLM;
	
	// PLLN (6) = factor is user defined
	tmp |= (((uint32_t) pllN) << 6) & RCC_PLLCFGR_PLLN;
	
#if defined(STM32F4XX)
	// VCO output frequency must be configured to 336MHz
	// PLLP (16) = 0 (factor = 2) for CPU frequency = 168MHz
	// PLLQ (24) = 7 (factor = 7) for 48MHz
	tmp |= (7UL << 24) & RCC_PLLCFGR_PLLQ;
#elif defined(STM32F2XX)
	// VCO output frequency must be configured to 240MHz
	// PLLP (16) = 0 (factor = 2) for CPU frequency = 120MHz
	// PLLQ (24) = 5 (factor = 5) for 48MHz
	tmp |= (5UL << 24) & RCC_PLLCFGR_PLLQ;
#	warning "this is not tested yet"
#else
#	error "This file is not supposed to be used with the given CPU (only STM32F2/4xx)"
#endif
	
	RCC->PLLCFGR = tmp;
	
	// enable pll
	RCC->CR |= RCC_CR_PLLON;
}

// ----------------------------------------------------------------------------
bool
xpcc::stm32::Clock::switchToPll(uint32_t waitCycles)
{
	uint32_t t = waitCycles;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {
		if (!(--t)) {
			return false;
		}
	}

#if defined(STM32F4XX)
	// APB2 84MHz, APB1 42MHz, AHB 168MHz, select PLL as source
	RCC->CFGR =
			(RCC->CFGR & 0xffff0000) | // Try to generate a halfword write
			((RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_SW_PLL) & 0x0000ffff);
#elif defined(STM32F2XX)
	// APB2 60MHz, APB1 30MHz, AHB 120MHz, select PLL as source
	RCC->CFGR =
			(RCC->CFGR & 0xffff0000) | // Try to generate a halfword write
			((RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_SW_PLL) & 0x0000ffff);
#	warning "this is not tested yet"
#else
#	error "This file is not supposed to be used with the given CPU (only STM32F2/4xx)"
#endif
	
	// Wait till the main PLL is used as system clock source
	while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	{
	}

	return true;
}
