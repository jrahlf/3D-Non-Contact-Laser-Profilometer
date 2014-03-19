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

#ifndef XPCC__GPIO_HPP
#define XPCC__GPIO_HPP

#include <stdint.h>
#include <xpcc/architecture/utils.hpp>

/**
 * \ingroup		architecture
 * \defgroup	gpio	General purpose input and/or output pins (GPIO)
 *
 * These macros/classes are used to create architecture independent
 * definitions for hardware pins which then can be used as template
 * parameters for miscellaneous device drivers.
 *
 * \warning	When you use the pins directly outside any device driver class you
 * 			have to remember to initialize them first. Call configure(),
 * 			setInput() or setOutput() before the first use, otherwise the
 * 			result is undefined and most likely not what you expect!
 *
 * Example:
 * \code
 * #include <xpcc/architecture/platform.hpp>
 * #include <xpcc/driver/software_spi.hpp>
 * #include <xpcc/driver/lcd/st7036.hpp>
 *
 * GPIO__OUTPUT(Clk, D, 7);
 * GPIO__OUTPUT(Mosi, D, 5);
 *
 * GPIO__OUTPUT(LcdCs, D, 4);
 * GPIO__OUTPUT(LcdRs, D, 0);
 *
 * // Create a new type for the software SPI for the display
 * typedef xpcc::SoftwareSpi< Clk, Mosi, xpcc::gpio::Unused > SpiInterface;
 *
 * // Create a instance of the ST7036 based display class
 * xpcc::St7036< SpiInterface,
 *               LcdCs,
 *               LcdRs > display;
 *
 * ...
 * display.initialize();
 * display.setPosition(0, 0);
 * display.write("Hallo Welt!");
 * \endcode
 *
 * This example can be compiled for every AVR device without any change!
 *
 * The pins don't need to initialized here because the initialize()
 * method of the display does this for us.
 *
 * \see xpcc::St7036
 *
 * Creating a simple flashing light:
 * \code
 * #include <xpcc/architecture/platform.hpp>
 * #include <xpcc/architecture/driver/delay.hpp>
 *
 * GPIO__OUTPUT(Led, B, 0);
 *
 * int
 * main(void)
 * {
 *     Led::setOutput();
 *     Led::set();
 *
 *     while (1)
 *     {
 *         Led::toggle();
 *         xpcc::delay_ms(500);
 *     }
 * }
 * \endcode
 *
 * This will generate nearly optimal code. As all methods are \c static and
 * \c inline no function call is generated but the call is mapped directly
 * to a \c sbi assembler instruction!
 *
 * The generated code for the example above (ATmega8):
 * \verbatim
 ...
82: 20 9a          sbi  0x04, 0        ; Led::setOutput();
84: 28 9a          sbi  0x05, 0        ; Led::set();

86: 41 e0          ldi  r20, 0x01      ; Prelude for toggle and delay
88: 29 e9          ldi  r18, 0x99
8a: 35 e1          ldi  r19, 0x15

8c: 85 b1          in   r24, 0x05      ; Led::toggle();
8e: 84 27          eor  r24, r20
90: 85 b9          out  0x05, r24

92: c9 01          movw r24, r18       ; xpcc::delay_ms(500);
94: 01 97          sbiw r24, 0x01
96: f1 f7          brne .-4            ; 0x94 <main+0x12>

98: f9 cf          rjmp .-14           ; 0x8c <main+0xa>
\endverbatim
 *
 * Or for the ATxmega128a1:
 * \verbatim
 ...
22e: 81 e0         ldi  r24, 0x01
230: 80 93 21 06   sts  0x0621, r24    ; Led::setOutput();
234: 80 93 25 06   sts  0x0625, r24    ; Led::set();

238: 41 e0         ldi  r20, 0x01      ; Prelude for toggle and delay
23a: 20 ee         ldi  r18, 0xE0
23c: 3e e2         ldi  r19, 0x2E

23e: 40 93 27 06   sts  0x0627, r20    ; Led::toggle();

242: c9 01         movw r24, r18       ; xpcc::delay_ms(500);
244: 01 97         sbiw r24, 0x01
246: f1 f7         brne .-4            ; 0x244 <main+0x16>

248: fa cf         rjmp .-12           ; 0x23e <main+0x10>
\endverbatim
 *
 * As you can see, no function call whatsoever is involved!
 *
 * \see		driver
 * \author	Fabian Greif
 */

namespace xpcc
{
	/**
	 * \brief	General purpose input and/or output pins
	 * \ingroup	gpio
	 */
	namespace gpio
	{
		/**
		 * \brief	Mode of an I/O-pin
		 * \ingroup	gpio
		 */
		enum Mode
		{
			INPUT,
			OUTPUT,
		};

		static const bool LOW = false;
		static const bool HIGH = true;

		/**
		 * \brief	Dummy implementation of an I/O pin
		 *
		 * This class can be used when a pin is not required. All functions
		 * are dummy functions which do nothing. Unused::read() will always
		 * return \c false.
		 *
		 * For example when
		 * creating a software SPI with the xpcc::SoftwareSpi class and
		 * the return channel (MISO - Master In Slave Out) is not needed, a
		 * good way is to use this class as a parameter when defining the
		 * SPI class.
		 *
		 * Example:
		 * \code
		 * #include <xpcc/architecture/driver/gpio.hpp>
		 * #include <xpcc/driver/software_spi.hpp>
		 *
		 * namespace pin
		 * {
		 *     GPIO__OUTPUT(Clk, D, 7);
		 *     GPIO__OUTPUT(Mosi, D, 5);
		 * }
		 *
		 * SoftwareSpi< pin::Clk, pin::Mosi, xpcc::gpio::Unused > Spi;
		 *
		 * ...
		 * Spi::write(0xaa);
		 * \endcode
		 *
		 * \ingroup	gpio
		 */
		class Unused
		{
		public:
			ALWAYS_INLINE static void
			setOutput()
			{
			}

			ALWAYS_INLINE static void
			setOutput(bool)
			{
			}

			ALWAYS_INLINE static void
			setInput()
			{
			}

			ALWAYS_INLINE static void
			set()
			{
			}

			ALWAYS_INLINE static void
			set(bool)
			{
			}

			ALWAYS_INLINE static void
			reset()
			{
			}

			ALWAYS_INLINE static void
			toggle()
			{
			}

			/// Returns \c false
			ALWAYS_INLINE static bool
			read()
			{
				return false;
			}
		};

		/**
		 * \brief	Invert a pin
		 *
		 * This template can be used the invert the logic level of a normal
		 * pin template.
		 *
		 * Example:
		 * \code
		 * #include <xpcc/architecture/driver/gpio.hpp>
		 * #include <xpcc/driver/software_spi.hpp>
		 *
		 * namespace pin
		 * {
		 *     GPIO__OUTPUT(Led, B, 0);
		 * }
		 *
		 * typedef xpcc::gpio::Invert< pin::Led > Led;
		 *
		 * ...
		 * Led::reset();
		 * \endcode
		 *
		 * \ingroup	gpio
		 */
		template <typename Pin>
		class Invert
		{
		public:
			ALWAYS_INLINE static void
			setOutput()
			{
				Pin::setOutput();
			}

			ALWAYS_INLINE static void
			setOutput(bool value)
			{
				Pin::setOutput(!value);
			}

			ALWAYS_INLINE static void
			setInput()
			{
				Pin::setInput();
			}

			ALWAYS_INLINE static void
			set()
			{
				Pin::reset();
			}

			ALWAYS_INLINE static void
			set(bool value)
			{
				Pin::set(!value);
			}

			ALWAYS_INLINE static void
			reset()
			{
				Pin::set();
			}

			ALWAYS_INLINE static void
			toggle()
			{
				Pin::toggle();
			}

			ALWAYS_INLINE static bool
			read()
			{
				return !Pin::read();
			}
		};

		/**
		 * \brief	Generic implementation of a Nibble composed of four
		 * 			independent pins.
		 *
		 * When possible preferred the GPIO__NIBBLE_LOW() or GPIO__NIBBLE_HIGH()
		 * macros over this class as they are much faster and require less code.
		 *
		 * \see		GPIO__NIBBLE_LOW(), GPIO__NIBBLE_HIGH()
		 * \ingroup	gpio
		 */
		template <typename T3 = Unused,
				  typename T2 = Unused,
				  typename T1 = Unused,
				  typename T0 = Unused>
		class Nibble
		{
		public:
			ALWAYS_INLINE static void
			setOutput()
			{
				T3::setOutput();
				T2::setOutput();
				T1::setOutput();
				T0::setOutput();
			}

			ALWAYS_INLINE static void
			setInput()
			{
				T3::setInput();
				T2::setInput();
				T1::setInput();
				T0::setInput();
			}

			/**
			 * \brief	Read data
			 *
			 * Only the lower four bits are used, the higher four bits will
			 * always be zero.
			 */
			static uint8_t
			read()
			{
				uint8_t value = 0;

				if (T3::read()) { value |= 0x08; }
				if (T2::read()) { value |= 0x04; }
				if (T1::read()) { value |= 0x02; }
				if (T0::read()) { value |= 0x01; }

				return value;
			}

			/**
			 * \brief	Write data
			 *
			 * Only the lower four bits are used.
			 */
			static void
			write(uint8_t data)
			{
				if (data & 0x08) { T3::set(); } else { T3::reset(); }
				if (data & 0x04) { T2::set(); } else { T2::reset(); }
				if (data & 0x02) { T1::set(); } else { T1::reset(); }
				if (data & 0x01) { T0::set(); } else { T0::reset(); }
			}
		};

		/**
		 * \brief	Create a 8-bit port from arbitrary pins.
		 *
		 * Be aware that this method is slow, because for every write or read
		 * cycle, every of the eight pins have to be read/written
		 * individually.
		 *
		 * This class is included here because it can be very useful sometimes,
		 * for example when connecting a LCD where speed is not the main
		 * concern.
		 *
		 * When possible preferred the GPIO__PORT() or GPIO__OCTET()
		 * macros over this class as they are much faster and require less code.
		 *
		 * \see		GPIO__PORT(), GPIO__OCTET()
		 * \ingroup	gpio
		 */
		template <typename T7 = Unused,
				  typename T6 = Unused,
				  typename T5 = Unused,
				  typename T4 = Unused,
				  typename T3 = Unused,
				  typename T2 = Unused,
				  typename T1 = Unused,
				  typename T0 = Unused>
		class Port
		{
		public:
			static void
			setOutput()
			{
				T7::setOutput();
				T6::setOutput();
				T5::setOutput();
				T4::setOutput();
				T3::setOutput();
				T2::setOutput();
				T1::setOutput();
				T0::setOutput();
			}

			static void
			setInput()
			{
				T7::setInput();
				T6::setInput();
				T5::setInput();
				T4::setInput();
				T3::setInput();
				T2::setInput();
				T1::setInput();
				T0::setInput();
			}

			static uint8_t
			read()
			{
				uint8_t value = 0;

				if (T7::read()) { value |= 0b10000000; }
				if (T6::read()) { value |= 0b01000000; }
				if (T5::read()) { value |= 0b00100000; }
				if (T4::read()) { value |= 0b00010000; }
				if (T3::read()) { value |= 0b00001000; }
				if (T2::read()) { value |= 0b00000100; }
				if (T1::read()) { value |= 0b00000010; }
				if (T0::read()) { value |= 0b00000001; }

				return value;
			}

			static void
			write(uint8_t data)
			{
				T7::set(data & 0b10000000);
				T6::set(data & 0b01000000);
				T5::set(data & 0b00100000);
				T4::set(data & 0b00010000);
				T3::set(data & 0b00001000);
				T2::set(data & 0b00000100);
				T1::set(data & 0b00000010);
				T0::set(data & 0b00000001);
			}
		};

		/**
		 * \brief	Create a 16-bit port from arbitrary pins.
		 *
		 * Be aware that this method is slow, because for every write or read
		 * cycle, every of the sixteen (!) pins have to be read/written
		 * individually.
		 *
		 * This class is included here because it can be very useful sometimes,
		 * for example when connecting a LCD where speed is not the main
		 * concern.
		 *
		 * When possible preferred the GPIO__PORT()
		 * macros over this class as they are much faster and require less code.
		 *
		 * \see		GPIO__PORT()
		 * \ingroup	gpio
		 */
		template <typename T15 = Unused,
				  typename T14 = Unused,
				  typename T13 = Unused,
				  typename T12 = Unused,
				  typename T11 = Unused,
				  typename T10 = Unused,
				  typename T9  = Unused,
				  typename T8  = Unused,
				  typename T7  = Unused,
				  typename T6  = Unused,
				  typename T5  = Unused,
				  typename T4  = Unused,
				  typename T3  = Unused,
				  typename T2  = Unused,
				  typename T1  = Unused,
				  typename T0  = Unused>
		class DoublePort
		{
		public:
			static void
			setOutput()
			{
				T15::setOutput();
				T14::setOutput();
				T13::setOutput();
				T12::setOutput();
				T11::setOutput();
				T10::setOutput();
				T9::setOutput();
				T8::setOutput();
				T7::setOutput();
				T6::setOutput();
				T5::setOutput();
				T4::setOutput();
				T3::setOutput();
				T2::setOutput();
				T1::setOutput();
				T0::setOutput();
			}

			static void
			setInput()
			{
				T15::setInput();
				T14::setInput();
				T13::setInput();
				T12::setInput();
				T11::setInput();
				T10::setInput();
				T9::setInput();
				T8::setInput();
				T7::setInput();
				T6::setInput();
				T5::setInput();
				T4::setInput();
				T3::setInput();
				T2::setInput();
				T1::setInput();
				T0::setInput();
			}

			static uint16_t
			read()
			{
				uint16_t value = 0;

				if (T15::read()) { value |= 0b1000000000000000; }
				if (T14::read()) { value |= 0b0100000000000000; }
				if (T13::read()) { value |= 0b0010000000000000; }
				if (T12::read()) { value |= 0b0001000000000000; }
				if (T11::read()) { value |= 0b0000100000000000; }
				if (T10::read()) { value |= 0b0000010000000000; }
				if (T9::read())  { value |= 0b0000001000000000; }
				if (T8::read())  { value |= 0b0000000100000000; }
				if (T7::read())  { value |= 0b0000000010000000; }
				if (T6::read())  { value |= 0b0000000001000000; }
				if (T5::read())  { value |= 0b0000000000100000; }
				if (T4::read())  { value |= 0b0000000000010000; }
				if (T3::read())  { value |= 0b0000000000001000; }
				if (T2::read())  { value |= 0b0000000000000100; }
				if (T1::read())  { value |= 0b0000000000000010; }
				if (T0::read())  { value |= 0b0000000000000001; }

				return value;
			}

			static void
			write(uint16_t data)
			{
				T15::set(data & 0b1000000000000000);
				T14::set(data & 0b0100000000000000);
				T13::set(data & 0b0010000000000000);
				T12::set(data & 0b0001000000000000);
				T11::set(data & 0b0000100000000000);
				T10::set(data & 0b0000010000000000);
				T9::set (data & 0b0000001000000000);
				T8::set (data & 0b0000000100000000);
				T7::set (data & 0b0000000010000000);
				T6::set (data & 0b0000000001000000);
				T5::set (data & 0b0000000000100000);
				T4::set (data & 0b0000000000010000);
				T3::set (data & 0b0000000000001000);
				T2::set (data & 0b0000000000000100);
				T1::set (data & 0b0000000000000010);
				T0::set (data & 0b0000000000000001);
			}
		};
	}
}

#ifdef __DOXYGEN__

namespace xpcc
{
	namespace gpio
	{
		/**
		 * \brief	Configuration possibilities of a general purpose pin
		 *
		 * This type is defined differently for every architecture as they
		 * provide different capabilities.
		 *
		 * \e NORMAL and \e PULLUP is the minimal interface that should be
		 * provided by any architecture.
		 *
		 * \ingroup	gpio
		 */
		enum Configuration
		{
			NORMAL,		///< standard operation (floating input, external signal needed!)
			PULLUP		///< enable the internal pull-up resistor
		};
	}
}

/**
 * \brief	Create a input/output pin type
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     configure(xpcc::gpio::Mode mode,
 *               xpcc::gpio::Configuration config = xpcc::gpio::NORMAL);
 *
 *     // configure pin as output
 *     static inline void
 *     setOutput();
 *
 *     // configure pin as output and set high or low
 *     static inline void
 *     setOutput(bool value);
 *
 *     // configure pin as input
 *     static inline void
 *     setInput();
 *
 *     // set output to high level
 *     static inline void
 *     set();
 *
 *     // set output to 'value'
 *     static inline void
 *     set(bool value);
 *
 *     // set output to low level
 *     static inline void
 *     reset();
 *
 *     // toggle output
 *     static inline void
 *     toggle();
 *
 *     // read input
 *     static inline bool
 *     read();
 * }
 * \endcode
 *
 * read() must not be called when configured as output, set(), reset() and
 * toggle() should not be called when configured as input!
 *
 * You should always prefer GPIO__OUTPUT() and GPIO__INPUT() over this class when
 * the wrapped pin as a distinct direction.
 *
 * \ingroup	gpio
 */
#define	GPIO__IO(name, port, pin)

/**
 * \brief	Create a output pin type
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     setOutput();
 *
 *     static inline void
 *     setOutput(bool value);
 *
 *     static inline void
 *     set();
 *
 *     static inline void
 *     set(bool value);
 *
 *     static inline void
 *     reset();
 *
 *     static inline void
 *     toggle();
 * }
 * \endcode
 *
 * Even if the wrapped pin can only be used as output it isn't configured that
 * way from the beginning. So remember to call \b setOutput() at startup!
 *
 * \ingroup	gpio
 */
#define GPIO__OUTPUT(name, port, pin)

/**
 * \brief	Create a input pin type
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     configure(xpcc::gpio::Configuration config = xpcc::gpio::NORMAL);
 *
 *     static inline void
 *     setInput();
 *
 *     static inline bool
 *     read();
 * }
 * \endcode
 *
 * Even if the wrapped pin can only be used as input it isn't configured that
 * way from the beginning. So remember to call \b setInput() at startup!
 *
 * \ingroup	gpio
 */
#define GPIO__INPUT(name, port, pin)

/**
 * \brief	Connect the lower four bits to a nibble
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     setOutput();
 *
 *     static inline void
 *     setInput();
 *
 *     // Only the lower four bits are written. The upper bits of data will
 *     // be discarded.
 *     static inline void
 *     write(uint8_t data);
 *
 *     // Value is in the lower four bits. Upper nibble will always be zero.
 *     static inline uint8_t
 *     read();
 * }
 * \endcode
 *
 * \see		xpcc::gpio::Nibble
 * \ingroup	gpio
 */
#define GPIO__NIBBLE_LOW(name, port)

/**
 * \brief	Connect the higher four bits to a nibble
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     setOutput();
 *
 *     static inline void
 *     setInput();
 *
 *     // The lower four bits are mapped to the upper four bits. The upper
 *     // nibble of data is discarded.
 *     static inline void
 *     write(uint8_t data);
 *
 *     // Value is in the lower four bits. Upper nibble will always be zero.
 *     static inline uint8_t
 *     read();
 * }
 * \endcode
 *
 * \see		xpcc::gpio::Nibble
 * \ingroup	gpio
 */
#define GPIO__NIBBLE_HIGH(name, port)

/**
 * \brief	Use a full 8-Bit port
 *
 * Create a new class with the following methods:
 * \code
 * class name
 * {
 * public:
 *     static inline void
 *     setOutput();
 *
 *     static inline void
 *     setInput();
 *
 *     static inline void
 *     write(uint8_t data);
 *
 *     static inline uint8_t
 *     read();
 * }
 * \endcode
 *
 * \see		xpcc::gpio::Port()
 * \ingroup	gpio
 */
#define GPIO__PORT(name, port)

#endif	// __DOXYGEN__

// TODO documentation
#define GPIO__IO2(name, id)				GPIO__IO(name, id)
#define GPIO__OUTPUT2(name, id)			GPIO__OUTPUT(name, id)
#define GPIO__INPUT2(name, id)			GPIO__INPUT(name, id)

#define GPIO__IO3(name, port, id)		GPIO__IO(name, port, id)
#define GPIO__OUTPUT3(name, port, id)	GPIO__OUTPUT(name, port, id)
#define GPIO__INPUT3(name, port, id)	GPIO__INPUT(name, port, id)

#endif // XPCC__GPIO_HPP
