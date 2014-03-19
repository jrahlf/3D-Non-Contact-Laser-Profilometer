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

#ifndef CAN_CONNECTOR_TEST_HPP
#define CAN_CONNECTOR_TEST_HPP

#include <unittest/testsuite.hpp>

#include "testing_can_connector.hpp"

class CanConnectorTest : public unittest::TestSuite
{
	void
	checkShortMessage(const xpcc::can::Message& message) const;
	
	uint8_t
    getPayloadLength(uint8_t offset) const;
	
	void
	checkFragmentedMessage(const xpcc::can::Message& message,
			uint8_t fragmentId) const;
	
	void
	createMessage(xpcc::can::Message& message,
			uint8_t fragmentId) const;
public:
    virtual void
    setUp();
    
    virtual void
    tearDown();
    
public:
    CanConnectorTest();
    
    void
    testSendShortMessageDirect();
    
    void
    testSendShortMessage();
    
    void
    testSendFragmentedMessage();
    
    void
    testReceiveShortMessage();
    
    void
    testReceiveFragmentedMessage();
    
private:
	TestingCanConnector *connector;
	FakeCanDriver *driver;
	
	xpcc::Header xpccHeader;
	uint32_t normalIdentifier;
	uint32_t fragmentedIdentifier;
	uint8_t messageCounter;
	
	uint8_t shortPayload[8];
	uint8_t fragmentedPayload[14];
};

#endif
