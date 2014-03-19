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

#include "../gpio.hpp"
#include "../device.h"

#include "usart_1.hpp"

#include <xpcc_config.hpp>



namespace
{
	GPIO__OUTPUT(TxdA9, A, 9);
	GPIO__INPUT(RxdA10, A, 10);
	
	GPIO__OUTPUT(TxdB6, B, 6);
	GPIO__INPUT(RxdB7, B, 7);
}
namespace
{
	static const uint32_t apbClk = STM32_APB2_FREQUENCY;	// APB2
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::Usart1::configurePins(Mapping mapping)
{
	// Enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Initialize IO pins
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
	if (mapping == REMAP_PA9_PA10) {
		TxdA9::setAlternateFunction(AF_USART1, xpcc::stm32::PUSH_PULL);
		RxdA10::setAlternateFunction(AF_USART1);
	}
	else {
		TxdB6::setAlternateFunction(AF_USART1, xpcc::stm32::PUSH_PULL);
		RxdB7::setAlternateFunction(AF_USART1);
	}
#elif defined(STM32F10X) || defined(STM32F10X_HD) || defined(STM32F10X_XL) \
	|| defined(STM32F10X_CL)
	AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_USART1_REMAP) | mapping;
	if (mapping == REMAP_PA9_PA10) {
		TxdA9::setAlternateFunction(xpcc::stm32::PUSH_PULL);
		RxdA10::setInput();
	}
	else {
		TxdB6::setAlternateFunction(xpcc::stm32::PUSH_PULL);
		RxdB7::setInput();
	}
#else
#  error "Unknown CPU Type. Please define STM32F10X_.., STM32F2XX, STM32F3XX or STM32F4XX"
#endif
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::Usart1::setBaudrate(uint32_t baudrate)
{
	// Enable clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	USART1->CR1 = 0;
	
	// Set baudrate
	USART1->BRR = calculateBaudrateSettings(apbClk, baudrate);
	
	// Transmitter & Receiver-Enable, 8 Data Bits, 1 Stop Bit
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART1->CR2 = 0;
	USART1->CR3 = 0;
	
	USART1->CR1 |= USART_CR1_UE;		// Uart Enable
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::Usart1::write(uint8_t data)
{
#if defined(STM32F3XX)
	while (!(USART1->ISR & USART_ISR_TXE)) {
		// wait until the data register becomes empty
	}

	USART1->TDR = data;
#else
	while (!(USART1->SR & USART_SR_TXE)) {
		// wait until the data register becomes empty
	}
	
	USART1->DR = data;
#endif
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::Usart1::write(const uint8_t *s, uint8_t n)
{
	while (n-- != 0) {
		write(*s++);
	}
}

// ----------------------------------------------------------------------------
bool
xpcc::stm32::Usart1::read(uint8_t& c)
{
#if defined(STM32F3XX)
	if (USART1->ISR & USART_ISR_RXNE)
	{
		c = USART1->RDR;
		return true;
	}
#else
	if (USART1->SR & USART_SR_RXNE)
	{
		c = USART1->DR;
		return true;
	}
#endif
	return false;
}

// ----------------------------------------------------------------------------
uint8_t
xpcc::stm32::Usart1::read(uint8_t *buffer, uint8_t n)
{
	for (uint8_t i = 0; i < n; ++i)
	{
		if (read(*buffer++)) {
			return i;
		}
	}
	
	return n;
}


