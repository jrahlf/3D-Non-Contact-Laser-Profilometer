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


#include <xpcc/communication/backend/tipc/tipc.hpp>

#include <xpcc/debug/logger/imp/std.hpp>

// set the Loglevel
#undef  XPCC_LOG_LEVEL
#define XPCC_LOG_LEVEL xpcc::log::DEBUG

int
main()
{
	XPCC_LOG_INFO << "########## XPCC TIPC RECEIVER Test ##########" << xpcc::flush;
	
	xpcc::TipcConnector tipc;
	
	tipc.addReceiverId(0x10);
	tipc.addEventId(0x01);
	
	while(1)
	{
		if (tipc.isPacketAvailable())
		{
			const xpcc::Header& header =  tipc.getPacketHeader();
			const xpcc::SmartPointer payload = tipc.getPacketPayload();
			
			XPCC_LOG_INFO
					<< XPCC_FILE_INFO << "has "
					<< ((header.destination != 0) ? "ACTION" : "EVENT")
					<< " from:" << (int)header.source
					<< " value:" << *(int*) payload.getPointer()
					<< xpcc::endl;
			
			if (header.destination != 0)
			{
				xpcc::Header ackHeader( xpcc::Header::REQUEST, true, header.source, header.destination, 0x01 );
				tipc.sendPacket(ackHeader);
			}
			
			tipc.dropPacket();
		}
		else {
			XPCC_LOG_DEBUG << XPCC_FILE_INFO << "has no packet" << xpcc::endl;
		}
		
		usleep(100000);
	}
	
	XPCC_LOG_INFO << "########## XPCC TIPC RAW Test END ##########" << xpcc::endl;
}
