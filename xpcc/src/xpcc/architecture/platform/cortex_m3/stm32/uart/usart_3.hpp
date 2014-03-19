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

#ifndef XPCC_STM32__USART_3_HPP
#define XPCC_STM32__USART_3_HPP

#include <stdint.h>
#include "uart_base.hpp"
#include "../device.h"

namespace xpcc
{
	namespace stm32
	{
		/**
		 * @brief		Universal synchronous/asynchronous receiver
		 * 				transmitter (USART3)
		 * 
		 * Simple unbuffered implementation.
		 * 
		 * @ingroup		stm32
		 */
		class Usart3 : public UartBase
		{
		public:
			Usart3(uint32_t baudrate)
			{
				setBaudrate(baudrate);
			}
			
			enum Mapping
			{
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
				REMAP_PB10_PB11,	///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#else
				REMAP_PB10_PB11 = AFIO_MAPR_USART3_REMAP_NOREMAP,		///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11 = AFIO_MAPR_USART3_REMAP_PARTIALREMAP,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9 = AFIO_MAPR_USART3_REMAP_FULLREMAP,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#endif
			};
			
			/**
			 * Configure the IO Pins for Usart3
			 */
			static void
			configurePins(Mapping mapping);
			
			/**
			 * \brief	Set baudrate
			 * \param	baudrate	desired baud rate
			 */
			static void
			setBaudrate(uint32_t baudrate);
			
			/**
			 * \brief	Send a single byte
			 */
			static void
			write(uint8_t data);
			
			/**
			 * \brief	Write a block of bytes
			 *
			 * \param	*buffer	Pointer to a buffer
			 * \param	n	Number of bytes to be read
			 */
			static void
			write(const uint8_t *buffer, uint8_t n);
			
			/**
			 * \brief	Read a single byte
			 */
			static bool
			read(uint8_t& c);
			
			/**
			 * \brief	Read a block of bytes
			 * 
			 * \param	*buffer	Pointer to a buffer big enough to storage \a n bytes
			 * \param	n	Number of bytes to be read
			 * 
			 * \return	Number of bytes which could be read, maximal \a n
			 */
			static uint8_t
			read(uint8_t *buffer, uint8_t n);
		};
		
		
		// --------------------------------------------------------------------
		/**
		 * \brief	Buffered Usart
		 * 
		 * This implementation uses a ringbuffer. The size of the ringbuffer
		 * can be changed in the \c project.cfg file.
		 * 
		 * Just add add a value for USART3_TX_BUFFER_SIZE and/or 
		 * USART3_RX_BUFFER_SIZE in the \c defines section. The size can
		 * be any value between 1 and 254.
		 * 
		 * Example:
		 * \code
		 * [defines]
		 * USART3_TX_BUFFER_SIZE = 20
		 * USART3_RX_BUFFER_SIZE = 30
		 * \endcode
		 * 
		 * \ingroup	stm32
		 */
		class BufferedUsart3 : public UartBase
		{
		public:
			/**
			 * Set baudrate.
			 * 
			 * \param	baudrate	Desired baud rate (e.g. 115200)
			 * \param	interruptPriority
			 * 			Interrupt vector priority (0=highest to 15=lowest)
			 * \param	blocking
			 * 			The write-function waits until a free slot is available
			 * 			in the send buffer.
			 */
			BufferedUsart3(uint32_t baudrate,
					uint32_t interruptPriority, bool blocking = true)
			{
				setBaudrate(baudrate, interruptPriority, blocking);
			}
			
			enum Mapping
			{
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
				REMAP_PB10_PB11,	///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#else
				REMAP_PB10_PB11 = AFIO_MAPR_USART3_REMAP_NOREMAP,		///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11 = AFIO_MAPR_USART3_REMAP_PARTIALREMAP,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9 = AFIO_MAPR_USART3_REMAP_FULLREMAP,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#endif
			};
			
			/**
			 * Configure the IO Pins for Usart3
			 */
			static void
			configurePins(Mapping mapping);
			
			/**
			 * Set baudrate.
			 * 
			 * \param	baudrate	Desired baud rate (e.g. 115200)
			 * \param	interruptPriority
			 * 			Interrupt vector priority (0=highest to 15=lowest)
			 * \param	blocking
			 * 			The write-function waits until a free slot is available
			 * 			in the send buffer.
			 */
			static void
			setBaudrate(uint32_t baudrate, uint32_t interruptPriority,
					bool blocking = true);
			
			/**
			 * \brief	Send a single byte
			 */
			static void
			write(uint8_t data);
			
			/**
			 * \brief	Write a block of bytes
			 *
			 * \param	*buffer	Pointer to a buffer
			 * \param	n	Number of bytes to be read
			 */
			static void
			write(const uint8_t *buffer, uint8_t n);
			
			/**
			 * \brief	Read a single byte
			 */
			static bool
			read(uint8_t& c);
			
			/**
			 * \brief	Read a block of bytes
			 * 
			 * \param	*buffer	Pointer to a buffer big enough to storage \a n bytes
			 * \param	n	Number of bytes to be read
			 * 
			 * \return	Number of bytes which could be read, maximal \a n
			 */
			static uint8_t
			read(uint8_t *buffer, uint8_t n);
			
			/**
			 * \brief	Empty the receive FIFO queue and USART buffer.
			 *
			 * \return	the size of the deleted FIFO queue.
			 */
			static uint8_t
			flushReceiveBuffer();
			
//			static uint8_t
//			flushTransmitBuffer();
		};

		// --------------------------------------------------------------------
		/**
		 * \brief	BufferedFlow Usart
		 * 
		 * Buffered UARAT with Hardware Flow Control. 
		 * 
		 * This implementation uses a ringbuffer. The size of the ringbuffer
		 * can be changed in the \c project.cfg file.
		 * 
		 * Just add add a value for USART3_TX_BUFFER_SIZE and/or 
		 * USART3_RX_BUFFER_SIZE in the \c defines section. The size can
		 * be any value between 1 and 254.
		 * 
		 * Example:
		 * \code
		 * [defines]
		 * USART3_TX_BUFFER_SIZE = 20
		 * USART3_RX_BUFFER_SIZE = 30
		 * \endcode
		 * 
		 * \ingroup	stm32
		 */
		class BufferedFlowUsart3 : public UartBase
		{
		public:
			/**
			 * Set baudrate.
			 * 
			 * \param	baudrate	Desired baud rate (e.g. 115200)
			 * \param	interruptPriority
			 * 			Interrupt vector priority (0=highest to 15=lowest)
			 * \param	blocking
			 * 			The write-function waits until a free slot is available
			 * 			in the send buffer.
			 */
			BufferedFlowUsart3(uint32_t baudrate,
					uint32_t interruptPriority, bool blocking = true)
			{
				setBaudrate(baudrate, interruptPriority, blocking);
			}
			
			enum Mapping
			{
#if defined(STM32F2XX) || defined(STM32F3XX) || defined(STM32F4XX)
				REMAP_PB10_PB11,	///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#else
				REMAP_PB10_PB11 = AFIO_MAPR_USART3_REMAP_NOREMAP,		///< TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
				REMAP_PC10_PC11 = AFIO_MAPR_USART3_REMAP_PARTIALREMAP,	///< TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
				REMAP_PD8_PD9 = AFIO_MAPR_USART3_REMAP_FULLREMAP,		///< TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
#endif
			};
			
			/**
			 * Configure the IO Pins for Usart3
			 */
			static void
			configurePins(Mapping mapping);
			
			/**
			 * Set baudrate.
			 * 
			 * \param	baudrate	Desired baud rate (e.g. 115200)
			 * \param	interruptPriority
			 * 			Interrupt vector priority (0=highest to 15=lowest)
			 * \param	blocking
			 * 			The write-function waits until a free slot is available
			 * 			in the send buffer.
			 */
			static void
			setBaudrate(uint32_t baudrate, uint32_t interruptPriority,
					bool blocking = true);
			
			/**
			 * \brief	Send a single byte
			 */
			static void
			write(uint8_t data);
			
			/**
			 * \brief	Write a block of bytes
			 *
			 * \param	*buffer	Pointer to a buffer
			 * \param	n	Number of bytes to be read
			 */
			static void
			write(const uint8_t *buffer, uint8_t n);
			
			/**
			 * \brief	Read a single byte
			 */
			static bool
			read(uint8_t& c);
			
			/**
			 * \brief	Read a block of bytes
			 * 
			 * \param	*buffer	Pointer to a buffer big enough to storage \a n bytes
			 * \param	n	Number of bytes to be read
			 * 
			 * \return	Number of bytes which could be read, maximal \a n
			 */
			static uint8_t
			read(uint8_t *buffer, uint8_t n);
			
			/**
			 * \brief	Empty the receive FIFO queue and USART buffer.
			 *
			 * \return	the size of the deleted FIFO queue.
			 */
			static uint8_t
			flushReceiveBuffer();
			
//			static uint8_t
//			flushTransmitBuffer();
		};

		
		
#if defined(STM32F2XX) || defined(STM32F4XX)
		// --------------------------------------------------------------------
		/**
		 * \brief		USART3 in SPI master mode
		 * 
		 * \ingroup		stm32
		 */
		class UsartSpi3 : public UartBase
		{
		private:
			// TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14
			// TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14
			// TX/PD8, RX/PD9, CK/PD10, CTS/PD11, RTS/PD12
			GPIO__OUTPUT(TxdB10, B, 10);
			GPIO__INPUT(RxdB11, B, 11);
			GPIO__OUTPUT(CkB12, B, 12);
			
			GPIO__OUTPUT(TxdD8, D, 8);
			GPIO__INPUT(RxdD9, D, 9);
			GPIO__OUTPUT(CkD10, D, 10);
			
			GPIO__OUTPUT(TxdC10, C, 10);
			GPIO__INPUT(RxdC11, C, 11);
			GPIO__OUTPUT(CkC12, C, 12);
			
		public:
			enum Mode
			{
				MODE_0 = 0,
				MODE_1 = USART_CR2_CPHA,
				MODE_2 = USART_CR2_CPOL,
				MODE_3 = USART_CR2_CPOL | USART_CR2_CPHA,
			};
			
			enum MappingTx
			{
				REMAP_PB10,
				REMAP_PC10,
				REMAP_PD8,
			};
			
			enum MappingRx
			{
				REMAP_PB11,
				REMAP_PC11,
				REMAP_PD9,
			};
			
			enum MappingCk
			{
				REMAP_PB12,
				REMAP_PC12,
				REMAP_PD10,
			};
			
			ALWAYS_INLINE static void
			configureTxPin(MappingTx mapping)
			{
				switch (mapping) {
					case REMAP_PB10: TxdB10::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
					case REMAP_PC10: TxdC10::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
					case REMAP_PD8:  TxdD8::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
				}
			}
			
			ALWAYS_INLINE static void
			configureRxPin(MappingRx mapping)
			{
				switch (mapping) {
					case REMAP_PB11: RxdB11::setAlternateFunction(AF_USART3); break;
					case REMAP_PC11: RxdC11::setAlternateFunction(AF_USART3); break;
					case REMAP_PD9:  RxdD9::setAlternateFunction(AF_USART3); break;
				}
			}
			
			ALWAYS_INLINE static void
			configureCkPin(MappingCk mapping)
			{
				switch (mapping) {
					case REMAP_PB12: CkB12::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
					case REMAP_PC12: CkC12::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
					case REMAP_PD10: CkD10::setAlternateFunction(AF_USART3, xpcc::stm32::PUSH_PULL); break;
				}
			}
			
			/**
			 * @brief	Initialize module in syncronous mode (SPI)
			 * 
			 * The bitrate is calculated in the same manner as for the
			 * asynchronous mode and is limited.
			 * 
			 * @param	bitrate		Desired Frequency of the SPI clock.
			 * @param	mode		Select the Spi Mode. Default is MODE_0.
			 * @param	over8		Using over8 sets the Oversampling down from 16
			 * 			to 8 however in SPI Mode this has effect on
			 * 			sample and hold time only, since Data are sampled
			 * 			at clock edges. Benefit of over8 is a doubled maximum
			 * 			bitrate.
			 */
			static void
			initialize(uint32_t bitrate, Mode mode = MODE_0, bool over8 = false);
			
			/**
			 * Transfer byte.
			 */
			static uint8_t
			write(uint8_t data);
		};
#endif
	}
}

#endif // XPCC_STM32__USART_3_HPP