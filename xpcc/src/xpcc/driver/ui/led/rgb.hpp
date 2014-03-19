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

#ifndef XPCC__PWM_RGB_LED_HPP
#define XPCC__PWM_RGB_LED_HPP

#include <stdint.h>
#include "led.hpp"

namespace xpcc
{
	namespace led
	{
		
		enum Color
		{
			RED,
			GREEN,
			BLUE
		};
		
		/**
		 * \brief Group three LEDs into a RGB controller
		 *
		 * \todo add RGB specific functions to this class.
		 *
		 * \author	Niklas Hauser
		 * \ingroup led
		 */
		class Rgb
		{
		private:
			Led* red;
			Led* green;
			Led* blue;
			
		public:
			Rgb(Led* red, Led* green, Led* blue)
			:	red(red), green(green), blue(blue)
			{
			}
			
			inline void
			setBrightness(uint16_t redValue, uint16_t greenValue, uint16_t blueValue)
			{
				red->setBrightness(redValue);
				green->setBrightness(greenValue);
				blue->setBrightness(blueValue);
			}
			
			inline uint16_t
			getBrightness(Color color)
			{
				switch (color) {
					case RED:
						return red->getBrightness();
					case GREEN:
						return green->getBrightness();
					case BLUE:
						return blue->getBrightness();
					default:
						return 0;
				}
			}
			
			inline bool
			isFading()
			{
				return (red->isFading() ||
						green->isFading() ||
						blue->isFading());
			}
			
			inline void
			fadeTo(uint16_t time, uint16_t redValue, uint16_t greenValue, uint16_t blueValue)
			{
				red->fadeTo(time, redValue);
				green->fadeTo(time, greenValue);
				blue->fadeTo(time, blueValue);
			}
			
			/// Must be called at least every ms
			inline void
			run()
			{
				red->run();
				green->run();
				blue->run();
			}
		};
	}
}

#endif	// XPCC__PWM_RGB_LED_HPP
