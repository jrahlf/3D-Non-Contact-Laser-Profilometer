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

#define	UNITTEST__RETURN_ON_FAIL
#include "dispatcher_test.hpp"

#include <xpcc/architecture/driver/test/testing_clock.hpp>

// ----------------------------------------------------------------------------
void
DispatcherTest::setUp()
{
	backend = new FakeBackend();
	postman = new FakePostman();
	dispatcher = new xpcc::Dispatcher(backend, postman);
	timeline = new Timeline();
	
	component1 = new TestingComponent1(dispatcher, timeline);
	component2 = new TestingComponent2(dispatcher, timeline);
	
	postman->component1 = component1;
	postman->component2 = component2;
}

void
DispatcherTest::tearDown()
{
	delete component2;
	delete component1;
	delete timeline;
	delete dispatcher;
	delete postman;
	delete backend;
}

// ----------------------------------------------------------------------------
void
DispatcherTest::testReceiveRequest()
{
	Message message(xpcc::Header(xpcc::Header::REQUEST, false, 1, 10, 0x10),
			xpcc::SmartPointer());
	
	backend->messagesToReceive.append(message);
	
	dispatcher->update();
	
	// message was delivered
	TEST_ASSERT_EQUALS(postman->messagesToDeliver.getSize(), 1U);
	TEST_ASSERT_TRUE(postman->messagesToDeliver.getFront() == message);
	
	// ACK has been send
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
	
	xpcc::Header ackHeader(xpcc::Header::REQUEST, true, 10, 1, 0x10);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header, ackHeader);
}

void
DispatcherTest::testReceiveRequestNoComponent()
{
	Message message(xpcc::Header(xpcc::Header::REQUEST, false, 11, 10, 0x10),
			xpcc::SmartPointer());
	
	backend->messagesToReceive.append(message);
	
	dispatcher->update();
	
	// message was delivered
	TEST_ASSERT_EQUALS(postman->messagesToDeliver.getSize(), 1U);
	TEST_ASSERT_TRUE(postman->messagesToDeliver.getFront() == message);
	
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}

// ----------------------------------------------------------------------------
void
DispatcherTest::testReceiveResponse()
{
	Message message(xpcc::Header(xpcc::Header::RESPONSE, false, 1, 10, 0x10),
			xpcc::SmartPointer());
	
	backend->messagesToReceive.append(message);
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(postman->messagesToDeliver.getSize(), 0U);
	
	// ACK has been send
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
	
	xpcc::Header ackHeader(xpcc::Header::RESPONSE, true, 10, 1, 0x10);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header, ackHeader);
}

void
DispatcherTest::testReceiveResponseNoComponent()
{
	Message message(xpcc::Header(xpcc::Header::RESPONSE, false, 11, 10, 0x10),
			xpcc::SmartPointer());
	
	backend->messagesToReceive.append(message);
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(postman->messagesToDeliver.getSize(), 0U);
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}

// ----------------------------------------------------------------------------
void
DispatcherTest::testEventReception()
{
	Message message(xpcc::Header(xpcc::Header::REQUEST, false, 0, 10, 0x20),
			xpcc::SmartPointer());
	
	backend->messagesToReceive.append(message);
	
	dispatcher->update();
	
	// message was delivered
	TEST_ASSERT_EQUALS(postman->messagesToDeliver.getSize(), 1U);
	TEST_ASSERT_TRUE(postman->messagesToDeliver.getFront() == message);
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 2U);
}

void
DispatcherTest::testEventTransmission()
{
	uint32_t payload = 0x12345678;
	component2->publishEvent(0x21, payload);
	
	dispatcher->update();
	
	// Event was delivered to the internal component
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::EVENT);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x21);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 4);
	
	// Event was also send to the backend (broadcast)
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 4U);
	TEST_ASSERT_EQUALS(
			backend->messagesSend.getFront().payload.get<uint32_t>(),
			0x12345678U);
	
	xpcc::Header header(xpcc::Header::REQUEST, false, 0, 2, 0x21);
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header, header);
}

void
DispatcherTest::testInternalActionCallNoParameter()
{
	component2->callAction(1, 0x10);
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x10);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
}

void
DispatcherTest::testInternalActionCallUnit16()
{
	uint16_t payload = 0x1234;
	component2->callAction(1, 0x11, payload);
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x11);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 2U);
	TEST_ASSERT_EQUALS(
			timeline->events.getFront().payload.get<uint16_t>(),
			0x1234U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
}

void
DispatcherTest::testInternalActionCallDirectResponse()
{
	xpcc::ResponseCallback callback(component2, &TestingComponent2::responseNoParameter);
	component2->callAction(1, 0x12, callback);
	
	dispatcher->update();
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 2U);
	
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x12);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::RESPONSE);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x30);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 2U);
}

void
DispatcherTest::testInternalActionCallDelayedResponse()
{
	xpcc::ResponseCallback callback(component2, &TestingComponent2::responseNoParameter);
	component2->callAction(1, 0x13, callback);
	
	dispatcher->update();
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x13);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	timeline->events.removeFront();
	
	// trigger the transmission of the response
	component1->update();
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::RESPONSE);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x30);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 2U);
}

void
DispatcherTest::testInternalActionCallWithRecall()
{
	uint16_t payload = 0x4321;
	component2->callAction(1, 0x14, payload);
	
	dispatcher->update();
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 2U);
	
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x14);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 2U);
	TEST_ASSERT_EQUALS(
			timeline->events.getFront().payload.get<uint16_t>(),
			0x4321U);
	
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x11);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 2U);
	TEST_ASSERT_EQUALS(
			timeline->events.getFront().payload.get<uint16_t>(),
			0x4321U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 2U);
}

void
DispatcherTest::testInternalActionBackAndForth()
{
	xpcc::ResponseCallback callback(component2, &TestingComponent2::responseCallAction);
	
	component2->callAction(1, 0x12, callback);
	
	uint16_t payload = 0x4321;
	component2->callAction(1, 0x14, payload);
	component1->callAction(2, 0x10);
	
	for (uint8_t i = 0; i < 10; ++i) {
		dispatcher->update();
	}
	
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 7U);
	
	// 1
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x12);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// 2
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x14);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 2U);
	TEST_ASSERT_EQUALS(
			timeline->events.getFront().payload.get<uint16_t>(),
			0x4321U);
	
	// 3
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x10);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// 4
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x11);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 2U);
	TEST_ASSERT_EQUALS(
			timeline->events.getFront().payload.get<uint16_t>(),
			0x4321U);
	
	// 5
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::RESPONSE);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x32);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// 6
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::ACTION);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x12);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// 7
	timeline->events.removeFront();
	TEST_ASSERT_TRUE(timeline->events.getFront().type == Timeline::RESPONSE);
	TEST_ASSERT_EQUALS(timeline->events.getFront().id, 0x30);
	TEST_ASSERT_EQUALS(timeline->events.getFront().component, 2);
	TEST_ASSERT_EQUALS(timeline->events.getFront().source, 1);
	TEST_ASSERT_EQUALS(timeline->events.getFront().payload.getSize(), 0U);
	
	// TODO should it be possible to disable this behavior
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 7U);
}

// ----------------------------------------------------------------------------
void
DispatcherTest::testActionRetransmissionWithAbort()
{
	component1->callAction(10, 0xf3);
	
	dispatcher->update();
	
	for (uint8_t i = 0; i < 3; i++)
	{
		TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header,
				xpcc::Header(xpcc::Header::REQUEST, false, 10, 1, 0xf3));
		
		backend->messagesSend.removeAll();
		
		// reset time so that the timeout is expired
		TestingClock::time += 100;
		
		dispatcher->update();
	}
		
	// transmission should be aborted by now
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}

// ----------------------------------------------------------------------------
void
DispatcherTest::testActionRetransmission()
{
	component1->callAction(10, 0xf3);
	
	dispatcher->update();
	
	for (uint8_t i = 0; i < 2; i++)
	{
		TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header,
				xpcc::Header(xpcc::Header::REQUEST, false, 10, 1, 0xf3));
		
		backend->messagesSend.removeAll();
		
		if (i == 0) {
			// reset time so that the timeout is expired
			TestingClock::time += 100;
		}
		
		dispatcher->update();
	}
	
	// send requested ACK
	backend->messagesToReceive.append(
			Message(xpcc::Header(xpcc::Header::REQUEST, true, 1, 10, 0xf3),
					xpcc::SmartPointer()));
	
	// reset time so that the timeout is expired
	TestingClock::time += 100;
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}

void
DispatcherTest::testResponseRetransmissionWithAbort()
{
	backend->messagesToReceive.append(
			Message(xpcc::Header(xpcc::Header::REQUEST, false, 1, 10, 0x12),
					xpcc::SmartPointer()));
	
	dispatcher->update();
	
	// message was delivered and processed
	TEST_ASSERT_EQUALS(timeline->events.getSize(), 1U);
	
	// ACK was send + Response was send
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 2U);
	
	// ACK
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header,
			xpcc::Header(xpcc::Header::REQUEST, true, 10, 1, 0x12));
	TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
	backend->messagesSend.removeFront();
	
	for (uint8_t i = 0; i < 3; i++)
	{
		TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header,
				xpcc::Header(xpcc::Header::RESPONSE, false, 10, 1, 0x12));
		
		backend->messagesSend.removeFront();
		
		// reset time so that the timeout is expired
		TestingClock::time += 100;
		
		dispatcher->update();
	}
	
	// transmission should be aborted by now
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}

void
DispatcherTest::testResponseRetransmission()
{
	backend->messagesToReceive.append(
			Message(xpcc::Header(xpcc::Header::REQUEST, false, 1, 10, 0x12),
					xpcc::SmartPointer()));
	
	dispatcher->update();
	
	// Remove ACK
	backend->messagesSend.removeFront();
	
	for (uint8_t i = 0; i < 2; i++)
	{
		TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 1U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().payload.getSize(), 0U);
		TEST_ASSERT_EQUALS(backend->messagesSend.getFront().header,
				xpcc::Header(xpcc::Header::RESPONSE, false, 10, 1, 0x12));
		
		backend->messagesSend.removeFront();
		
		if (i == 0) {
			// reset time so that the timeout is expired
			TestingClock::time += 100;
		}
		
		dispatcher->update();
	}
	
	// send requested ACK
	backend->messagesToReceive.append(
			Message(xpcc::Header(xpcc::Header::RESPONSE, true, 1, 10, 0x12),
					xpcc::SmartPointer()));
	
	// reset time so that the timeout is expired if still active
	TestingClock::time += 100;
	
	dispatcher->update();
	
	TEST_ASSERT_EQUALS(backend->messagesSend.getSize(), 0U);
}
