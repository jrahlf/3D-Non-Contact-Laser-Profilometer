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

#ifndef XPCC__SIEMENS_S65_HPP
#define XPCC__SIEMENS_S65_HPP

#include <xpcc/architecture/driver/delay.hpp>

#include "buffered_graphic_display.hpp"

// to enable this define add it to your ``project.cfg`` and
// include <xpcc_config.hpp> before including this file.
#ifndef S65_LPC_ACCELERATED
#	define S65_LPC_ACCELERATED 0
#endif

namespace xpcc
{
	/**
	 * \brief	Driver for Siemens S65 displays
	 *
	 * The controller is a Hitachi HD66773 which can drive color displays of up
	 * ot 132 x 176 pixels in 18 bit colour.
	 *
	 * The portrait mode is a bit more 'native' for this display because
	 * xpcc::BufferedGraphicDisplay requests that the vertical resolution is
	 * dividable by 8.
	 *
	 * In portrait mode the connector is at the bottom.
	 * In landscape mode the connector is at the right border.
	 *
	 * \ingroup	lcd
	 */

	// common for landscape and portrait
	template <typename SPI, typename CS, typename RS, typename Reset>
	class SiemensS65Common
	{
	protected:
		ALWAYS_INLINE void
		writeCmd(uint8_t reg, uint16_t param);

		ALWAYS_INLINE void
		writeReg(uint8_t reg);

		ALWAYS_INLINE void
		writeData(uint16_t param);

		ALWAYS_INLINE void
		lcdCls(uint16_t colour);

		ALWAYS_INLINE void
		lcdSettings(bool landscape);

	};

	template <typename SPI, typename CS, typename RS, typename Reset>
	class SiemensS65Portrait :
		public BufferedGraphicDisplay<132, 176>,
		public SiemensS65Common<SPI, CS, RS, Reset>
	{
	public:
		void
		initialize();

		/**
		 * \brief	Update the display with the content of the RAM buffer
		 */
		virtual void
		update();

	};

		/**
		 * The display in landscape mode does not match the required
		 * alignment of BufferedGraphicDisplay which requests that
		 * the vertical resolution can be divided by 8.
		 *
		 */
	template <typename SPI, typename CS, typename RS, typename Reset>
	class SiemensS65Landscape :
		public BufferedGraphicDisplay<176, 136>,
		public SiemensS65Common<SPI, CS, RS, Reset>
	{
	public:
		void
		initialize();

		/**
		 * \brief	Update the display with the content of the RAM buffer
		 */
		virtual void
		update();
	};

}

#include "siemens_s65_impl.hpp"

#endif // XPCC__SIEMENS_S65_HPP
