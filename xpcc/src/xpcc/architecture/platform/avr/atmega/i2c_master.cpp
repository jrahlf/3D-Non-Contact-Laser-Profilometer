// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2012, Roboterclub Aachen e.V.
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "i2c_master.hpp"

// uncomment to debug your driver using simple uart
/*
#include <xpcc/architecture/platform.hpp>
#define DEBUG(x) xpcc::atmega::BufferedUart0::write(x)
/*/
#define DEBUG(x)
//*/

// ----------------------------------------------------------------------------
namespace
{
	// parameter advice
	static xpcc::i2c::Delegate::NextOperation nextOperation;
	
	// buffer management
	static uint8_t *readPointer;
	static const uint8_t *writePointer;
	static std::size_t readBytesLeft;
	static std::size_t writeBytesLeft;
	
	// delegating
	static xpcc::i2c::Delegate *delegate(0);
	static xpcc::atmega::I2cMaster::ErrorState errorState(xpcc::atmega::I2cMaster::NO_ERROR);
	static bool newSession;
	
	// helper functions
	static ALWAYS_INLINE void
	initializeWrite(xpcc::i2c::Delegate::Writing w) {
		writePointer = w.buffer;
		writeBytesLeft = w.size;
		nextOperation = static_cast<xpcc::i2c::Delegate::NextOperation>(w.next);
	}
	
	static ALWAYS_INLINE void
	initializeRead(xpcc::i2c::Delegate::Reading r) {
		readPointer = r.buffer;
		readBytesLeft = r.size;
		nextOperation = static_cast<xpcc::i2c::Delegate::NextOperation>(r.next);
	}
	
	static ALWAYS_INLINE void
	initializeStopAfterAddress() {
		//writePointer = ..;
		writeBytesLeft = 0;
		nextOperation = xpcc::i2c::Delegate::STOP_OP;
	}
	
	static ALWAYS_INLINE void
	initializeRestartAfterAddress() {
		//writePointer = ..;
		writeBytesLeft = 0;
		nextOperation = xpcc::i2c::Delegate::RESTART_OP;
	}
}

// ----------------------------------------------------------------------------
/// TWI state machine interrupt handler
ISR(TWI_vect)
{
	switch(TW_STATUS)
	{
		case TW_START:
			// START has been transmitted
			// Fall through...
		case TW_REP_START:
			{
				DEBUG('s');
				// REPEATED START has been transmitted
				xpcc::i2c::Delegate::Starting s = delegate->started();
				uint8_t address;
				switch (s.next)
				{
					case xpcc::i2c::Delegate::READ_OP:
						address = (s.address & 0xfe) | xpcc::i2c::READ;
						initializeRead(delegate->reading());
						break;
						
					case xpcc::i2c::Delegate::WRITE_OP:
						address = (s.address & 0xfe) | xpcc::i2c::WRITE;
						initializeWrite(delegate->writing());
						break;
						
					default:
					case xpcc::i2c::Delegate::STOP_OP:
						address = (s.address & 0xfe) | xpcc::i2c::WRITE;
						initializeStopAfterAddress();
						break;
						
					case xpcc::i2c::Delegate::RESTART_OP:
						address = (s.address & 0xfe) | xpcc::i2c::WRITE;
						initializeRestartAfterAddress();
						break;
				}
			
				// Load data register with TWI slave address
				TWDR = address;
				// clear interrupt flag to send address
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
			}
			break;
			
		case TW_MT_SLA_ACK:
			// SLA+W has been transmitted and ACK received
			// Fall through...
		case TW_MT_DATA_ACK:
			// Data byte has been transmitted and ACK received
			if (writeBytesLeft != 0)
			{
				TWDR = *writePointer++;
				DEBUG('0' + writeBytesLeft);
				--writeBytesLeft;
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
				DEBUG('A');
			}
			else
			{
				switch (nextOperation)
				{
					case xpcc::i2c::Delegate::WRITE_OP:
						DEBUG('C');
						initializeWrite(delegate->writing());
						TWDR = *writePointer++;
						--writeBytesLeft;
						TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
						DEBUG('A');
						break;
						
					case xpcc::i2c::Delegate::RESTART_OP:
						DEBUG('R');
						TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
						break;
						
					default:
						DEBUG('S');
						TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
						delegate->stopped(xpcc::i2c::Delegate::NORMAL_STOP);
						delegate = 0;
						break;
				}
			}
			break;
			
		case TW_MR_DATA_ACK:
			*readPointer++ = TWDR;
			DEBUG('0' + readBytesLeft);
			--readBytesLeft;
			
			// Fall through...
		case TW_MR_SLA_ACK:
			// SLA+R has been transmitted and ACK received
			// See if last expected byte will be received ...
			if (readBytesLeft > 0) {
				// Send ACK after reception
				TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
				DEBUG('a');
			}
			else {
				// Send NACK after next reception
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
				DEBUG('n');
			}
			break;
			
		case TW_MR_DATA_NACK:
			// Data byte has been received and NACK transmitted
			// => Transfer finished
			*readPointer++ = TWDR;
			DEBUG('0' + readBytesLeft);
			--readBytesLeft;
			
			switch (nextOperation)
			{
				case xpcc::i2c::Delegate::RESTART_OP:
					DEBUG('r');
					TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
					break;
					
				default:
				case xpcc::i2c::Delegate::STOP_OP:
					DEBUG('S');
					TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
					delegate->stopped(xpcc::i2c::Delegate::NORMAL_STOP);
					delegate = 0;
					break;
			}
			break;
			
		case TW_MT_SLA_NACK:	// SLA+W transmitted, NACK received
		case TW_MR_SLA_NACK:	// SLA+R transmitted, NACK received
			errorState = xpcc::atmega::I2cMaster::ADDRESS_NACK;
			DEBUG('A');
			newSession = false;
		case TW_MT_DATA_NACK:	// data transmitted, NACK received
			if (newSession) errorState = xpcc::atmega::I2cMaster::DATA_NACK;
			DEBUG('d');
			newSession = false;
			// generate a stop condition
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
		case TW_MT_ARB_LOST:	// arbitration lost in SLA+W or data
//		case TW_MR_ARB_LOST:	// arbitration lost in SLA+R or NACK
			if (newSession) errorState = xpcc::atmega::I2cMaster::ARBITRATION_LOST;
			DEBUG('d');
			newSession = false;
		default:
			if (newSession) errorState = xpcc::atmega::I2cMaster::UNKNOWN_ERROR;
			DEBUG('r');
			newSession = false;
			xpcc::atmega::I2cMaster::reset(true);
			break;
	}
	DEBUG('\n');
}

// ----------------------------------------------------------------------------
void
xpcc::atmega::I2cMaster::initialize(uint8_t twbr, uint8_t twps)
{
	reset();
	// Initialize TWI clock
	TWBR = twbr;
	TWSR = twps & 0x03;
	// Load data register with default content; release SDA
	TWDR = 0xff;
	// Enable TWI peripheral with interrupt disabled
	TWCR = (1 << TWEN);
}

void
xpcc::atmega::I2cMaster::reset(bool error)
{
	DEBUG('T');
	writeBytesLeft = 0;
	readBytesLeft = 0;
	if (delegate) delegate->stopped(
			error ?	xpcc::i2c::Delegate::ERROR_CONDITION :
					xpcc::i2c::Delegate::SOFTWARE_RESET);
	delegate = 0;
}

uint8_t
xpcc::atmega::I2cMaster::getErrorState()
{
	return static_cast<uint8_t>(errorState);
}

bool
xpcc::atmega::I2cMaster::start(xpcc::i2c::Delegate *delegate)
{
	if (!::delegate && delegate && delegate->attaching())
	{
		::delegate = delegate;
		newSession = true;
		DEBUG('Y');
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
		
		return true;
	}
	return false;
}

bool
xpcc::atmega::I2cMaster::startSync(xpcc::i2c::Delegate *delegate)
{
	if (!::delegate && delegate && delegate->attaching())
	{
		::delegate = delegate;
		newSession = true;
		DEBUG('Y');
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);
		
		// the compiler generates out of order code here
		// so we introduce a memory barrier
		// to make sure, ::delegate is set _before_ entering a deadlock
		__asm__ volatile ("" ::: "memory");
		
		while (::delegate)
			;
		DEBUG('f');
		
		return true;
	}
	return false;
}
