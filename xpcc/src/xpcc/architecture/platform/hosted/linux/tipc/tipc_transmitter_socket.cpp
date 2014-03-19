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

#include "tipc_transmitter_socket.hpp"
#include "header.hpp"

#include <sys/socket.h>
#include <unistd.h> // close()
#include <linux/tipc.h>
#include <xpcc/container/smart_pointer.hpp>

#include <iostream>

#include <xpcc/debug/logger.hpp>

#undef  XPCC_LOG_LEVEL
#define XPCC_LOG_LEVEL xpcc::log::WARNING

// ----------------------------------------------------------------------------
xpcc::tipc::TransmitterSocket::TransmitterSocket() :
	socketDescriptor_ ( socket (AF_TIPC, SOCK_RDM,0) )
{
}

// ----------------------------------------------------------------------------
xpcc::tipc::TransmitterSocket::~TransmitterSocket()
{
	// Close the socket
	close( this->socketDescriptor_ );
}

// ----------------------------------------------------------------------------
void 
xpcc::tipc::TransmitterSocket::transmitPayload(
		unsigned int typeId,
		unsigned int instanceId,
		const uint8_t* packet,
		size_t length)
{
	int sendToResult	=	0;
	
	sockaddr_tipc tipcToAddresse;

	// Initialize the toAddress struct of TIPC. For further documentation have a look
	// into the Multicast-Demo which is included into the TIPC-Demo package.
	tipcToAddresse.family				=	AF_TIPC;
	tipcToAddresse.addrtype				=	TIPC_ADDR_MCAST;
	tipcToAddresse.addr.nameseq.type	=	typeId;
	tipcToAddresse.addr.name.domain		=	0;
	tipcToAddresse.addr.nameseq.lower	=	instanceId;
	tipcToAddresse.addr.nameseq.upper	=	instanceId;


	// In the following section the payload will be connected to the tipc-header
	Header header;
	header.size = length;

	// Allocate memory for whole packet (header plus payload)
	xpcc::SmartPointer tipcPacketPointer ( sizeof(Header) + length );

	// Put things together - first the tipc-header and then the payload
	memcpy( tipcPacketPointer.getPointer(), &header, sizeof(Header) );
	memcpy( tipcPacketPointer.getPointer()+sizeof(Header), packet, length);

	sendToResult	=	sendto(	this->socketDescriptor_,
								(void*)tipcPacketPointer.getPointer(),
								length + sizeof(Header),
								0,
								(struct sockaddr*)&tipcToAddresse,
								(size_t)sizeof(tipcToAddresse));

//	XPCC_LOG_DEBUG << XPCC_FILE_INFO
//			<< " tid=" << (int)typeId
//			<< " iid=" << (int)instanceId
//			<< " value=" << tipcPacketPointer;
//	XPCC_LOG_DEBUG << xpcc::flush;

	// Check if the sending failed
	if (sendToResult < 0) {
		XPCC_LOG_ERROR << XPCC_FILE_INFO << __FUNCTION__ << "on transmit" << xpcc::flush;

		// Throw an exception because a connection error cannot be handled here.
		// Just closing and opening the socket again is not suitable because one
		// should be informed about such bad errors to eliminate the cause and not
		// the effect!
		// TODO: Maybe write a suitable exception, here!!!
//		throw Exception("TIPC Transmitter falied to send!");
	}
}

// ----------------------------------------------------------------------------
uint32_t
xpcc::tipc::TransmitterSocket::getPortId()
{
	/**
	 * int getsockname(int sockfd, struct sockaddr *localaddr, socklen_t *addrlen)
	 *
	 * Gets the port ID of the socket.
	 * Comments:
	 * - The use of "name" in getsockname() can be confusing, as the routine does
	 * not actually return the TIPC names or name sequences that have been bound to
	 * the socket.
	 */

	struct sockaddr_tipc addr;
	socklen_t sz = sizeof(addr);

	if ( getsockname( this->socketDescriptor_, (struct sockaddr *)&addr, &sz) < 0 ) {
		XPCC_LOG_ERROR
				<< XPCC_FILE_INFO
				<< "Failed to get sock address"
				<< xpcc::endl;
		return 0;
	}

	return addr.addr.id.ref;
}
