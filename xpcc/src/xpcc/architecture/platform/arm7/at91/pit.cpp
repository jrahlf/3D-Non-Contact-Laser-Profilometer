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

#include "pit.hpp"
#include "device.h"

// ----------------------------------------------------------------------------
void
xpcc::at91::Pit::start(uint32_t period)
{
	period &= 0x000fffff;
	
	if (period == 0) {
		period = F_CPU / 16 / 1000;
	}
	else if (period > 0x000fffff) {
		period = 0x000fffff;
	}
	
	AT91C_BASE_PITC->PITC_PIMR = 
			(AT91C_BASE_PITC->PITC_PIMR & AT91C_PITC_PITIEN) |
			 AT91C_PITC_PITEN |
			 period;
}

// ----------------------------------------------------------------------------
void
xpcc::at91::Pit::stop()
{
	AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITEN;
}

// ----------------------------------------------------------------------------
void
xpcc::at91::Pit::registerInterruptHandler(Aic::Handler function)
{
	Aic::registerHandler(AT91C_ID_SYS, function);
	Aic::enableInterrupt(AT91C_ID_SYS);
	
	AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITIEN;
}

// ----------------------------------------------------------------------------
void
xpcc::at91::Pit::removeInterruptHandler()
{
	AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;
	
	Aic::disableInterrupt(AT91C_ID_SYS);
}
