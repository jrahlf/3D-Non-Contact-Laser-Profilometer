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

#ifndef XPCC__ST7036_HPP
#define XPCC__ST7036_HPP

#include "character_display.hpp"

namespace xpcc
{
	/**
	 * \brief	Driver for ST7036 based LC-displays
	 * 
	 * \todo	make this class adaptable to other line counts!
	 * \todo	documentation
	 * 
	 * Available defines:
	 * - ST7036_VOLTAGE = 3|5
	 * 
	 * \see		Lcd
	 * 
	 * \author	Fabian Greif
	 * \ingroup	lcd
	 */
	template <typename SPI, typename CS, typename RS, unsigned int Width, unsigned int Heigth>
	class St7036 : public CharacterDisplay
	{
	public:
		/// Constructor
		St7036();
		
		/**
		 * \brief	Initialize the display
		 * 
		 * The display needs some time to initialize after startup. You have
		 * to wait at least 50 ms until calling this method.
		 */
		virtual void
		initialize();
		
		virtual void
		writeRaw(char c);
		
		virtual void
		execute(Command command);
		
		virtual void
		setCursor(uint8_t column, uint8_t line);
		
		// TODO
		//void
		//setContrast();
	
	protected:
		void
		writeCommand(uint8_t command);
	};
}

#include "st7036_impl.hpp"

#endif // XPCC__ST7036_HPP
