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

#ifndef XPCC__IOSTREAM_HPP
#define XPCC__IOSTREAM_HPP

#include <xpcc/architecture/utils.hpp>

#include "iodevice.hpp"
#include "iodevice_wrapper.hpp"

namespace xpcc
{
	/**
	 * \brief 	This Formats all primary types into a string stream for
	 * 			output or it reads values from a input and converts them to
	 * 			a given type;
	 *
	 * \ingroup io
	 * \author	Martin Rosekeit <martin.rosekeit@rwth-aachen.de>
	 */
	class IOStream
	{
	public :
		/**
		 * \brief	Constructor
		 * 
		 * \param	device	device to write the stream to
		 * 
		 * \code
		 *	MyIODevice device;
		 *	IOStream stream( device );
		 * \endcode
		 */
		IOStream(IODevice& device);

		inline IOStream&
		write(char c)
		{
			this->device->write(c);
			return *this;
		}
		
		inline IOStream&
		flush()
		{
			this->device->flush();
			this->mode = Mode::Ascii;
			return *this;
		}
		
		/// set the output mode to Mode::Binary style for \c char and \c char*
		ALWAYS_INLINE IOStream&
		bin()
		{
			this->mode = Mode::Binary;
			return *this;
		}

		/// set the output mode to Mode::Hexadecimal style for \c char and \c char*
		ALWAYS_INLINE IOStream&
		hex()
		{
			this->mode = Mode::Hexadecimal;
			return *this;
		}

		/// set the output mode to Mode::Ascii style for \c char and \c char*
		ALWAYS_INLINE IOStream&
		ascii()
		{
			this->mode = Mode::Ascii;
			return *this;
		}
		
		
		IOStream&
		operator << (const unsigned char& v)
		{
			if (this->mode == Mode::Ascii) {
				this->writeInteger(static_cast<uint16_t>(v));
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(v);
			}
			else {
				this->writeHex(v);
			}
			return *this;
		}

		IOStream&
		operator << (const char& v)
		{
			if (this->mode == Mode::Ascii) {
				this->device->write(v);
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(v);
			}
			else {
				this->writeHex(v);
			}
			return *this;
		}
		
		ALWAYS_INLINE IOStream&
		operator << (const uint16_t& v)
		{
			if (this->mode == Mode::Ascii) {
				this->writeInteger(v);
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(static_cast<uint8_t>(v >> 8));
				this->writeBin(static_cast<uint8_t>(v & 0xff));
			}
			else {
				this->writeHex(static_cast<uint8_t>(v >> 8));
				this->writeHex(static_cast<uint8_t>(v & 0xff));
			}
			return *this;
		}

		ALWAYS_INLINE IOStream&
		operator << (const int16_t& v)
		{
			if (this->mode == Mode::Ascii) {
				this->writeInteger(v);
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(static_cast<uint8_t>(v >> 8));
				this->writeBin(static_cast<uint8_t>(v & 0xff));
			}
			else {
				this->writeHex(static_cast<uint8_t>(v >> 8));
				this->writeHex(static_cast<uint8_t>(v & 0xff));
			}
			return *this;
		}

		ALWAYS_INLINE IOStream&
		operator << (const uint32_t& v)
		{
			if (this->mode == Mode::Ascii) {
				this->writeInteger(v);
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(static_cast<uint8_t>(v >> 24));
				this->writeBin(static_cast<uint8_t>(v >> 16));
				this->writeBin(static_cast<uint8_t>(v >> 8));
				this->writeBin(static_cast<uint8_t>(v & 0xff));
			}
			else {
				this->writeHex(static_cast<uint8_t>(v >> 24));
				this->writeHex(static_cast<uint8_t>(v >> 16));
				this->writeHex(static_cast<uint8_t>(v >> 8));
				this->writeHex(static_cast<uint8_t>(v & 0xff));
			}
			return *this;
		}

		ALWAYS_INLINE IOStream&
		operator << (const int32_t& v)
		{
			if (this->mode == Mode::Ascii) {
				this->writeInteger(v);
			}
			else if (this->mode == Mode::Binary) {
				this->writeBin(static_cast<uint8_t>(v >> 24));
				this->writeBin(static_cast<uint8_t>(v >> 16));
				this->writeBin(static_cast<uint8_t>(v >> 8));
				this->writeBin(static_cast<uint8_t>(v & 0xff));
			}
			else {
				this->writeHex(static_cast<uint8_t>(v >> 24));
				this->writeHex(static_cast<uint8_t>(v >> 16));
				this->writeHex(static_cast<uint8_t>(v >> 8));
				this->writeHex(static_cast<uint8_t>(v & 0xff));
			}
			return *this;
		}
		
#if defined(XPCC__OS_OSX)
		// For APPLE 'int64_t' is of type 'int'. Therefore there is no
		// function here for the default type 'long int'. As 'long int' has the same
		// width as 'int64_t' we just use a typedef here.
		ALWAYS_INLINE IOStream&
		operator << (const long int& v)
		{
			this->writeInteger(static_cast<int64_t>(v));
			return *this;
		}
		
		ALWAYS_INLINE IOStream&
		operator << (const long unsigned int& v)
		{
			this->writeInteger(static_cast<uint64_t>(v));
			return *this;
		}
#endif
		
#if defined(XPCC__CPU_ARM) || defined(XPCC__CPU_AVR32)
		// For ARM 'int32_t' is of type 'long'. Therefore there is no
		// function here for the default type 'int'. As 'int' has the same
		// width as 'int32_t' we just use a typedef here.
		ALWAYS_INLINE IOStream&
		operator << (const int& v)
		{
			this->writeInteger(static_cast<int32_t>(v));
			return *this;
		}
		
		ALWAYS_INLINE IOStream&
		operator << (const unsigned int& v)
		{
			this->writeInteger(static_cast<uint32_t>(v));
			return *this;
		}
#endif
		
// The 64-bit types on the AVR are extremely slow and are
// therefore excluded here
#if !defined(XPCC__CPU_AVR)
		ALWAYS_INLINE IOStream&
		operator << (const uint64_t& v)
		{
			this->writeInteger(v);
			return *this;
		}
		
		ALWAYS_INLINE IOStream&
		operator << (const int64_t& v)
		{
			this->writeInteger(v);
			return *this;
		}
#endif
		
		ALWAYS_INLINE IOStream&
		operator << (const float& v)
		{
			this->writeFloat(v);
			return *this;
		}

		ALWAYS_INLINE IOStream&
		operator << (const double& v)
		{
#if defined(XPCC__CPU_AVR)
			this->writeFloat(static_cast<float>(v));
#else
			this->writeDouble(v);
#endif
			return *this;
		}

		IOStream&
		operator << (const char* s)
		{
			if( this->mode == Mode::Ascii ) {
				this->device->write(s);
			}
			else if( this->mode == Mode::Binary ) {
				this->writeBin(s);
			}
			else {
				this->writeHex(s);
			}
			return *this;
		}
		
		// write the hex value of a pointer
		IOStream&
		operator << (const void* p);
		
		ALWAYS_INLINE IOStream&
		operator << (IOStream& (*function)(IOStream&))
		{
			return function(*this);
		}
		
		/**
		 * @brief	Simple printf() implemenation
		 * 
		 * The format string is composed of zero or more directives: ordinary
		 * characters (not %), which are copied unchanged to the output stream;
		 * and conversion specifications, each of which results in fetching zero
		 * or more subsequent arguments. Each conversion specification is
		 * introduced by the % character. The arguments must properly correspond
		 * (after type promotion) with the conversion specifier. After the %,
		 * the following appear in sequence:
		 * 
		 * - Zero or more of the following flags:
		 *   - \c - A negative field width flag; the converted value is to be
		 *     left adjusted on the field boundary. The converted value is
		 *     padded on the right with blanks, rather than on the left with
		 *     blanks.
		 *   - ' ' (space) A blank should be left before a positive number
		 *     produced by a signed conversion (d).
		 *   - \c + A sign must always be placed before a number produced by a
		 *     signed conversion. A + overrides a space if both are used.
		 *   - An optional decimal digit string specifying a minimum field width.
		 *     If the converted value has fewer characters than the field width,
		 *     it will be padded with spaces on the left (or right, if the
		 *     left-adjustment flag has been given) to fill out the field width.
		 *   - An optional h, l or ll length modifier, that specifies that the argument
		 *     for the d, u, or x conversion is a 8-bit ("h"), 32-bit ("l") or
		 *     64-bit ("ll") rather than 16-bit.
		 * 
		 * The conversion specifiers and their meanings are:
		 * - \c c	char (8-bit)
		 * - \c s	string (char *)
		 * - \c p	pointer (void *)
		 * - \c d	signed  decimal
		 * - \c u	unsigned decimal
		 * - \c x	hex
		 * - \c %	%
		 * 
		 * Combined with the length modifiers you get:
		 * - \c d	signed 16-bit
		 * - \c ld	signed 32-bit
		 * - \c lld	signed 64-bit (not yet)
		 * - \c u	unsigned 16-bit
		 * - \c lu	unsigned 32-bit
		 * - \c llu	unsigned 64-bit (not yet)
		 * - \c hx	 8-bit hex
		 * - \c x	16-bit hex
		 * - \c lx	32-bit hex
		 * - \c llx	64-bit hex (not yet)
		 * 
		 * Examples, given \c -100 as argument in the right type:
		 * \code
		 * %c:  
		 * %s:  
		 * %p:  0x0100
		 * %d:  -100
		 * %ld: -100
		 * %u:  65436
		 * %lu: 
		 * %hx: 0x9c
		 * %x:  0xff9c
		 * %lx: 0xffffff9c
		 * \endcode
		 * 
		 * - \c s The "char *" argument is expected to be a pointer to an array
		 *   of character type (pointer to a string). Characters from the array
		 *   are written up to (but not including) a terminating NULL character;
		 *   if a precision is specified, no more than the number specified are
		 *   written. If a precision is given, no null character need be present;
		 *   if the precision is not specified, or is greater than the size of
		 *   the array, the array must contain a terminating NULL character.
		 * - \c % A % is written. No argument is converted. The complete conversion
		 *   specification is "%%".
		 * 
		 * In no case does a non-existent or small field width cause truncation
		 * of a numeric field; if the result of a conversion is wider than the
		 * field width, the field is expanded to contain the conversion result.
		 * 
		 * @param	fmt		Format string
		 */
		IOStream&
		printf(const char* fmt, ...);
		
	protected :
		void
		writeInteger(int16_t value);
		
		void
		writeInteger(uint16_t value);
		
		void
		writeInteger(int32_t value);
		
		void
		writeInteger(uint32_t value);
		
		void
		writeInteger(int64_t value);
		
		void
		writeInteger(uint64_t value);
		
		void
		writeHex(const char* s);
		
		void
		writeBin(const char* s);

		void
		writeHexNibble(uint8_t nibble);
		
		void
		writeHex(uint8_t value);
		
		void
		writeBin(uint8_t value);

		void
		writeFloat(const float& value);
		
#if !defined(XPCC__CPU_AVR)
		void
		writeDouble(const double& value);
#endif

	private :
		enum class
		Mode
		{
			Ascii,
			Hexadecimal,
			Binary
		};
		
		IOStream(const IOStream&);

		IOStream&
		operator =(const IOStream&);
		
	private:
		IODevice* const	device;
		Mode mode;
	};
	
	/**
	 * \brief  Flushes the output stream.
	 *
	 * This manipulator simply calls the stream's flush() member function.
	 * \ingroup io
	*/
	inline IOStream&
	flush(IOStream& ios)
	{
		return ios.flush();
	}
	
	/**
	 * \brief  Write a newline and flush the stream.
	 *
	 * \ingroup io
	 */
	inline IOStream&
	endl(IOStream& ios)
	{
		return flush(ios.write('\n'));
	}

	/**
	 * \brief  set the output mode to Mode::Binary style for \b char and \b char*
	 *
	 * \ingroup io
	 */
	inline IOStream&
	bin(IOStream& ios)
	{
		return ios.bin();
	}

	/**
	 * \brief  set the output mode to Mode::Hexadecimal style for \b char and \b char*
	 *
	 * \ingroup io
	 */
	inline IOStream&
	hex(IOStream& ios)
	{
		return ios.hex();
	}

	/**
	 * \brief  set the output mode to Mode::Ascii style for \b char and \b char*
	 *
	 * \ingroup io
	 */
	inline IOStream&
	ascii(IOStream& ios)
	{
		return ios.ascii();
	}

	/**
	 * \brief	Set the foreground colour on ANSI terminals.
	 *
	 * \ingroup io
	 */
	inline IOStream&
	black(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('0');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	red(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('1');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	green(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('2');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	yellow(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('3');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	blue(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('4');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	magenta(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('1');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	cyan(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('6');
		ios.write('m');
		return ios;
	}

	inline IOStream&
	white(IOStream& ios)
	{
		ios.write('\033');
		ios.write('[');
		ios.write('3');
		ios.write('7');
		ios.write('m');
		return ios;
	}

};


#endif // XPCC__IOSTREAM_HPP
