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

#ifndef XPCC__SIEMENS_M55_HPP
#error	"Don't include this file directly, use 'siemens_m55.hpp' instead!"
#endif

uint8_t initData_lm15[] =    // initialisation data for LCD
{
0xF4 , 0x90 , 0xB3 , 0xA0 , 0xD0,
0xF0 , 0xE2 , 0xD4 , 0x70 , 0x66 , 0xB2 , 0xBA , 0xA1 , 0xA3 , 0xAB , 0x94 , 0x95 , 0x95 , 0x95,
0xF5 , 0x90,
0xF1 , 0x00 , 0x10 , 0x22 , 0x30 , 0x45 , 0x50 , 0x68 , 0x70 , 0x8A , 0x90 , 0xAC , 0xB0 , 0xCE , 0xD0,
0xF2 , 0x0F , 0x10 , 0x20 , 0x30 , 0x43 , 0x50 , 0x66 , 0x70 , 0x89 , 0x90 , 0xAB , 0xB0 , 0xCD , 0xD0,
0xF3 , 0x0E , 0x10 , 0x2F , 0x30 , 0x40 , 0x50 , 0x64 , 0x70 , 0x87 , 0x90 , 0xAA , 0xB0 , 0xCB , 0xD0,
0xF4 , 0x0D , 0x10 , 0x2E , 0x30 , 0x4F , 0x50,
0xF5 , 0x91,
0xF1 , 0x01 , 0x11 , 0x22 , 0x31 , 0x43 , 0x51 , 0x64 , 0x71 , 0x86 , 0x91 , 0xA8 , 0xB1 , 0xCB , 0xD1,
0xF2 , 0x0F , 0x11 , 0x21 , 0x31 , 0x42 , 0x51 , 0x63 , 0x71 , 0x85 , 0x91 , 0xA6 , 0xB1 , 0xC8 , 0xD1,
0xF3 , 0x0B , 0x11 , 0x2F , 0x31 , 0x41 , 0x51 , 0x62 , 0x71 , 0x83 , 0x91 , 0xA4 , 0xB1 , 0xC6 , 0xD1,
0xF4 , 0x08 , 0x11 , 0x2B , 0x31 , 0x4F , 0x51,
0x80 , 0x94,
0xF5 , 0xA2,
0xF4 , 0x60,
0xF0 , 0x40 , 0x50 , 0xC0,
0xF4, 0x70
};



// ----------------------------------------------------------------------------
template <typename SPI, typename CS, typename RS, typename Reset>
void
xpcc::SiemensM55<SPI, CS, RS, Reset>::initialize()
{
	// CS pin
	CS::setOutput(true);

	// RS pin
	RS::setOutput(false);

	// Reset pin
	Reset::setOutput(false);

	lcdSettings();

	this->clear();
	//	this->update();
}

template <typename SPI, typename CS, typename RS, typename Reset>
void
xpcc::SiemensM55<SPI, CS, RS, Reset>::lcdSettings() {
	// Hardware reset is low from initialize
	xpcc::delay_ms(10);
	Reset::set();
	xpcc::delay_ms(10);

	RS::set();	// command mode
	CS::reset();	// select display

	for(uint8_t ii = 0; ii < sizeof(initData_lm15); ++ii)
	{
	  SPI::write(initData_lm15[ii]);   // send initialization data
	}
	xpcc::delay_ms(1);
	CS::set();	// deactivate LCD CS

	xpcc::delay_ms(1);
	CS::reset();	// activate LCD CS

	SPI::write(0xF0);
	SPI::write(0x81);
	SPI::write(0xF4);
	SPI::write(0xB3);
	SPI::write(0xA0);

	SPI::write(0xF0);
	SPI::write(0x06);
	SPI::write(0x10);
	SPI::write(0x20);
	SPI::write(0x30);
	SPI::write(0xF5);
	SPI::write(0x0F);
	SPI::write(0x1C);
	SPI::write(0x2F);
	SPI::write(0x34);

	CS::set();	 // deactivate LCD CS
	RS::reset();	// set LCD to data mode

	xpcc::delay_ms(10);


	static uint8_t contrast = 22;

	RS::set();
	CS::reset();

	SPI::write(0xF4);
	SPI::write(0xB0 | (contrast >> 4));
	SPI::write(0xA0 | (contrast & 0x0F));

	CS::set();

	lcdCls(0x00f0);
}

template <typename SPI, typename CS, typename RS, typename Reset>
void
xpcc::SiemensM55<SPI, CS, RS, Reset>::lcdCls(uint16_t colour) {
	RS::reset();
	CS::reset();

	uint8_t c1 = colour >> 8;
	uint8_t c2 = colour & 0xff;
	for (uint16_t ii = 0; ii < 101*80; ++ii)
	{
		SPI::write(c1);
		SPI::write(c2);
	}

	CS::set();
}

template <typename SPI, typename CS, typename RS, typename Reset>
void
xpcc::SiemensM55<SPI, CS, RS, Reset>::update() {

	// WRITE MEMORY
	RS::set();
	CS::reset();

	// cursor
    SPI::write(0xF0);
    SPI::write(0x00 | (0x06 & 0x0F));
    SPI::write(0x10 | (0x06 >> 4));
    SPI::write(0x20 | (0x00 & 0x0F));
    SPI::write(0x30 | (0x00 >> 4));

	CS::set();

	RS::reset();
	CS::reset();

	const uint16_t mask1Blank  = 0x0000; // xxxx RRRR GGGG BBBB
	const uint16_t mask1Filled = 0x0FFf; // xxxx RRRR GGGG BBBB
	const uint8_t fill_h = mask1Filled >> 8;
	const uint8_t fill_l = mask1Filled & 0xff;
	const uint8_t blank_h = mask1Blank >> 8;
	const uint8_t blank_l = mask1Blank & 0xff;

	for (uint8_t y = 0; y < 80; ++y)
	{
		for (uint8_t x = 0; x < 101; ++x)
		{
			// group of 8 black-and-white pixels
			uint8_t group = this->buffer[x][y/8];

			if (group & (1 << (y % 8)))
			{
				SPI::write(fill_h);
				SPI::write(fill_l);
			}
			else
			{
				SPI::write(blank_h);
				SPI::write(blank_l);
			}
		}
	}

	CS::set();
}
