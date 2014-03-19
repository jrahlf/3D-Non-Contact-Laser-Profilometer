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

#include <xpcc/math/saturated/saturated.hpp>

#include "saturated_test.hpp"

void
SaturatedTest::testSigned8bit()
{
	//TEST_FAIL("TODO");
	
}
	
void
SaturatedTest::testUnsigned8bit()
{
	xpcc::Saturated<uint8_t> x;
	xpcc::Saturated<uint8_t> y(100);
	
	TEST_ASSERT_EQUALS(x.getValue(), 0);
	TEST_ASSERT_EQUALS(y.getValue(), 100);
	
	x = 200;
	
	TEST_ASSERT_EQUALS(x.getValue(), 200);
	
	x += y;
	
	TEST_ASSERT_EQUALS(x.getValue(), 255);
	
	x = 10;
	y = 20;
	x -= y;
	
	TEST_ASSERT_EQUALS(x.getValue(), 0);
	
	xpcc::Saturated<uint8_t> z;
	
	x = 20;
	y = 10;
	
	z = x + y;
	
	TEST_ASSERT_EQUALS(x.getValue(), 20);
	TEST_ASSERT_EQUALS(y.getValue(), 10);
	TEST_ASSERT_EQUALS(z.getValue(), 30);
	
	z = x - y;
	
	TEST_ASSERT_EQUALS(x.getValue(), 20);
	TEST_ASSERT_EQUALS(y.getValue(), 10);
	TEST_ASSERT_EQUALS(z.getValue(), 10);
	
	y = z - x;
	
	TEST_ASSERT_EQUALS(x.getValue(), 20);
	TEST_ASSERT_EQUALS(y.getValue(), 0);
	TEST_ASSERT_EQUALS(z.getValue(), 10);
	
	x = -z;
	
	TEST_ASSERT_EQUALS(x.getValue(), 0);
	
	y = 200;
	//x = abs(y);
	x = y;
	x.absolute();
	
	TEST_ASSERT_EQUALS(x.getValue(), 200);
}
