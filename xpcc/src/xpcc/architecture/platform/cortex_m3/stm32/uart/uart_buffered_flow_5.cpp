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

#include "uart_5.hpp"

#include <xpcc/architecture/driver/atomic.hpp>
#include <xpcc_config.hpp>

// RTS and CTS pin and port must be declated in project.cfg
#if (UART5_RTS_PIN  != -1) && \
	(UART5_CTS_PIN  != -1)
	    
/*	(UART5_RTS_PORT != -1) && \
    (UART5_CTS_PORT != -1) */

#if defined(STM32F10X_HD) || defined(STM32F10X_XL) || defined(STM32F10X_CL) || \
	defined(STM32F2XX) || defined(STM32F3XXV) || defined(STM32F4XX)
// only devices with 100 pins have Port D and therefore UART5


namespace
{
	static xpcc::atomic::Queue<char, UART5_RX_BUFFER_SIZE> rxBuffer;
	static xpcc::atomic::Queue<char, UART5_TX_BUFFER_SIZE> txBuffer;
	
	static bool isBlocking = true;
}

namespace
{
	GPIO__OUTPUT(TxdC12, C, 12);
	GPIO__INPUT(RxdD2, D, 2);
}
namespace
{
	static const uint32_t apbClk = STM32_APB1_FREQUENCY;	// APB1
}

namespace
{
	GPIO__OUTPUT(RTS, UART5_RTS_PORT, UART5_RTS_PIN);
	GPIO__INPUT (CTS, UART5_CTS_PORT, UART5_CTS_PIN);
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::BufferedFlowUart5::configurePins(Mapping mapping)
{
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
	
	// Initialize IO pins
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
	(void) mapping;		// avoid compiler warning
	
	TxdC12::setAlternateFunction(AF_UART5, xpcc::stm32::PUSH_PULL);
	RxdD2::setAlternateFunction(AF_UART5);
#else
	(void) mapping;		// avoid compiler warning
	
	TxdC12::setAlternateFunction(xpcc::stm32::PUSH_PULL);
	RxdD2::setInput();
#endif
}

// ----------------------------------------------------------------------------
#if defined(STM32F3XX)
	#define USART_SR_RXNE USART_ISR_RXNE
	#define USART_SR_TXE  USART_ISR_TXE

	// new register names for STM32F3 series
	#define UART5_SR \
		UART5->ISR
	#define UART5_RDR \
		UART5->RDR
	#define UART5_TDR \
		UART5->TDR

#else
	#define UART5_SR \
		UART5->SR
	#define UART5_RDR \
		UART5->DR
	#define UART5_TDR \
		UART5->DR
#endif

extern "C" void
UART5_IRQHandler()
{
	uint32_t state = UART5_SR;
	
	// Read Data Register not empty 
	if (state & USART_SR_RXNE)
	{
		// First save the errors TODO
		// error |= USART5_STATUS & (USART_FERR_bm | USART_BUFOVF_bm | USART_PERR_bm);
		
		// Then read the buffer (read from DR clears the RXNE flag)
		uint8_t data = UART5_RDR;
		
		if (!rxBuffer.push(data))
		{
			// TODO Error handling if internal RX buffer is full.
		}
		
		// If the internal buffer is now nearly full set RTS to high to inhibit
		// remote from sending new data. As the remote may not stop instantenously
		// it is important to stop before the buffer is completely full.
		if (rxBuffer.isNearlyFull()) {
			RTS::set();
		}
	} // USART_SR_RXNE
	
	// Transmit Data Register empty
	if (state & USART_SR_TXE)
	{
		if (txBuffer.isEmpty())
		{
			// transmission finished, disable DRE interrupt
			UART5->CR1 &= ~USART_CR1_TXEIE;
		}
		else {
			// check if CTS is low which indicates that the remote device is ready
			// to accept data
			if (CTS::read())
			{
				// RTS output of remote device high: do not send new data.
				// Disable DRE interrupt
				UART5->CR1 &= ~USART_CR1_TXEIE;

				// TODO: Setup interrupt for rising edge of CTS to resume sending 
				// when remote device is ready again.
			}
			else
			{
				// RTS of remote device low: ready to receive new data.
				// get one byte from buffer and write it to the UART buffer
				// which starts the transmission
				UART5_TDR = txBuffer.get();
				txBuffer.pop();
			} // CTS
		} // txBuffer
	} // USART_SR_TXE
} // IRQHandler

// ----------------------------------------------------------------------------
void
xpcc::stm32::BufferedFlowUart5::setBaudrate(uint32_t baudrate,
		uint32_t interruptPriority, bool blocking)
{
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
	
	isBlocking = blocking;
	
	UART5->CR1 = 0;
	
	// Set vector priority
	NVIC_SetPriority(UART5_IRQn, interruptPriority);
	
	// Enable USART in the interrupt controller and enable receive ready interrupt
	NVIC->ISER[UART5_IRQn / 32] = 1 << (UART5_IRQn & 0x1F);
	
	UART5->CR1 |= USART_CR1_RXNEIE;
	
	// Set baudrate
	UART5->BRR = calculateBaudrateSettings(apbClk, baudrate);
	
	// Transmitter & Receiver-Enable, 8 Data Bits, 1 Stop Bit
	UART5->CR1 |= USART_CR1_TE | USART_CR1_RE;
	UART5->CR2 = 0;
	UART5->CR3 = 0;
	
	UART5->CR1 |= USART_CR1_UE;		// Uart Enable
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::BufferedFlowUart5::write(const uint8_t *s, uint8_t n)
{
	while (n-- != 0) {
		write(*s++);
	}
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::BufferedFlowUart5::write(uint8_t c)
{
	while ( !txBuffer.push(c) ) {
		if (!isBlocking) {
			return;
		}
	}
	
	// Disable interrupts while enabling the transmit interrupt
	atomic::Lock lock;
	
	// Transmit Data Register Empty Interrupt Enable
	UART5->CR1 |= USART_CR1_TXEIE;
}

// ----------------------------------------------------------------------------
bool
xpcc::stm32::BufferedFlowUart5::read(uint8_t& c)
{
	if (rxBuffer.isNearlyEmpty()) {
		// When the buffer is nearly empty allow remote device to send again.
		// This will create a continous flow of data.
		RTS::reset();
	}

	// Small hack: When the STM stopped transmission due to a high CTS signal try to resume
	// transmission now when CTS is low again and there is something to send.
	// TODO: can be removed if CTS interrupt is included.

	if (!CTS::read() && !txBuffer.isEmpty()) {
		// enable DRE interrupt to resume transmission
		USART2->CR1 |= USART_CR1_RXNEIE;
	}

	if (rxBuffer.isEmpty()) {
		// no data in buffer anymore
		RTS::reset();
		return false;
	}
	else {
		// at least one byte in buffer
		c = rxBuffer.get();
		rxBuffer.pop();
		
		return true;
	}
}

// ----------------------------------------------------------------------------
uint8_t
xpcc::stm32::BufferedFlowUart5::read(uint8_t *buffer, uint8_t n)
{
	uint_fast8_t i = 0;
	for (; i < n; ++i)
	{
		if (rxBuffer.isEmpty()) {
			RTS::reset();
			return i;
		}
		else {
			*buffer++ = rxBuffer.get();
			rxBuffer.pop();
			
			if (rxBuffer.isNearlyEmpty()) {
				// When the buffer is nearly empty allow remote device to send again.
				// This will create a continous flow of data.
				RTS::reset();
			}
		}
	}
	
	return i;
}

uint8_t
xpcc::stm32::BufferedFlowUart5::flushReceiveBuffer()
{
	uint_fast8_t i = 0;
	while (!rxBuffer.isEmpty()) {
		rxBuffer.pop();
		++i;
	}
//	unsigned char c;
//	while (USART5_STATUS & USART_RXCIF_bm)
//		c = USART5_DATA;
	
	return i;
}

//uint8_t
//xpcc::stm32::BufferedFlowUart5::flushTransmitBuffer()
//{
//	uint8_t i(0);
//	while(!txBuffer.isEmpty()) {
//		txBuffer.pop();
//		++i;
//	}
//
//	return i;
//}

#endif

#endif