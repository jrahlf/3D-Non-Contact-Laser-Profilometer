// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
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

#include "tipc_transmitter.hpp"
#include "header.hpp"

#include <xpcc/debug/logger.hpp>

// TODO: IMPORTANT! If the TIPC-Module is not loaded terminate with appropriate
// error message!!!!

// ----------------------------------------------------------------------------
xpcc::tipc::Transmitter::Transmitter( ) :
	tipcTransmitterSocket_()
{
}

// ----------------------------------------------------------------------------
xpcc::tipc::Transmitter::~Transmitter()
{
}

// ----------------------------------------------------------------------------
void 
xpcc::tipc::Transmitter::transmitRequest( uint8_t destination, const SmartPointer& payload )
{
	this->tipcTransmitterSocket_.transmitPayload(
			REQUEST_OFFSET + destination + TYPE_ID_OFFSET,
			0,
			payload.getPointer(),
			payload.getSize() );
}

// ----------------------------------------------------------------------------
void
xpcc::tipc::Transmitter::transmitEvent( uint8_t event, const SmartPointer& payload )
{
	this->tipcTransmitterSocket_.transmitPayload(
			EVENT_OFFSET + event + TYPE_ID_OFFSET,
			0,
			payload.getPointer(),
			payload.getSize() );
}

// ----------------------------------------------------------------------------
uint32_t
xpcc::tipc::Transmitter::getPortId()
{
	return this->tipcTransmitterSocket_.getPortId();
}
