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

#include <xpcc/math/filter/ramp.hpp>

#include "ramp_test.hpp"

void
RampTest::testConstructor()
{
	xpcc::filter::Ramp<int16_t> ramp(3, 4);
	
	TEST_ASSERT_TRUE(ramp.isTargetReached());
	TEST_ASSERT_EQUALS(ramp.getValue(), 0);
}

void
RampTest::testRamp()
{
	xpcc::filter::Ramp<int16_t> ramp(3, 4);
	
	ramp.update();
	TEST_ASSERT_EQUALS(ramp.getValue(), 0);
	
	ramp.setTarget(20);
	TEST_ASSERT_EQUALS(ramp.getValue(), 0);
	TEST_ASSERT_FALSE(ramp.isTargetReached());
	
	for (int i = 0; i < 6; ++i) {
		ramp.update();
		TEST_ASSERT_EQUALS(ramp.getValue(), i * 3 + 3);
	}
	TEST_ASSERT_FALSE(ramp.isTargetReached());
	
	ramp.update();
	TEST_ASSERT_EQUALS(ramp.getValue(), 20);
	TEST_ASSERT_TRUE(ramp.isTargetReached());
	
	ramp.update();
	TEST_ASSERT_EQUALS(ramp.getValue(), 20);
	TEST_ASSERT_TRUE(ramp.isTargetReached());
	
	ramp.setTarget(-50);
	TEST_ASSERT_EQUALS(ramp.getValue(), 20);
	TEST_ASSERT_FALSE(ramp.isTargetReached());
	
	for (int i = 0; i < 17; ++i) {
		ramp.update();
		TEST_ASSERT_EQUALS(ramp.getValue(), 20 - (i + 1) * 4);
	}
	TEST_ASSERT_FALSE(ramp.isTargetReached());
	
	ramp.update();
	TEST_ASSERT_EQUALS(ramp.getValue(), -50);
	TEST_ASSERT_TRUE(ramp.isTargetReached());
	
	ramp.update();
	TEST_ASSERT_EQUALS(ramp.getValue(), -50);
	TEST_ASSERT_TRUE(ramp.isTargetReached());
	
}
