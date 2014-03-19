#ifndef XPCC__SEVEN_SEGMENT_DISPLAY_HPP
#define XPCC__SEVEN_SEGMENT_DISPLAY_HPP

#include <stdint.h>
#include <xpcc/architecture/driver/gpio.hpp>

namespace xpcc {
	namespace sevenSegment {

		// config: left to right, right to left.

		static const uint8_t segA  = (1 << 5);
		static const uint8_t segB  = (1 << 7);
		static const uint8_t segC  = (1 << 4);
		static const uint8_t segD  = (1 << 2);
		static const uint8_t segE  = (1 << 0);
		static const uint8_t segF  = (1 << 3);
		static const uint8_t segG  = (1 << 1);
		static const uint8_t segDp = (1 << 6);

		static const
		uint8_t segmentLutHex[] =
		{
				segA | segB | segC | segD | segE | segF, // 0
				segB | segC,                             // 1
				segA | segB | segG | segE | segD,        // 2
				segA | segB | segC | segD | segG,        // 3
				segF | segG | segB | segC,               // 4
				segA | segF | segG | segC | segD,        // 5
				segA | segF | segG | segC | segD | segE, // 6
				segA | segB | segC,                      // 7
				segA | segB | segC | segD | segE | segF | segG, // 8
				segA | segB | segC | segD        | segF | segG, // 9
				segA | segB | segC        | segE | segF | segG, // A
				              segC | segD | segE | segF | segG, // b
			    segA               | segD | segE | segF,        // C
			           segB | segC | segD | segE |        segG, // d
			    segA |               segD | segE | segF | segG, // E
			    segA                      | segE | segF | segG, // F
		};

		/**
		 * \brief	Seven Segment Display Driver
		 *
		 * This driver makes seven segment displays that consists of several
		 * digits and are connected to daisy-chained shift registers (74xx595)
		 * easy to use.
		 *
		 * It because the Load signal must be strobe when all data is shifted out
		 * this blocks until the SPI has finished.
		 */
		template<
			typename Spi,
			typename Load,
			uint8_t DIGITS=1 >
		class SevenSegmentDisplay
		{
		public:
			/**
			 * \brief	Initialise the display
			 */
			static void
			initialize();

			/**
			 * \brief	Write a decimal value to the display
			 */
			static void
			write(uint32_t val);

			/**
			 * \brief	Write a hexadecimal value to the display
			 */
			static void
			writeHex(uint32_t val);
		};

	}  // namespace sevenSegment
}  // namespace xpcc

#include "seven_segment_impl.hpp"

#endif // XPCC__SEVEN_SEGMENT_DISPLAY_HPP
