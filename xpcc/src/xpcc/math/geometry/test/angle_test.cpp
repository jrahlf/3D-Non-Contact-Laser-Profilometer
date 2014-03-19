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

#include <xpcc/math/geometry/angle.hpp>

#include "angle_test.hpp"

void
AngleTest::testNormalize()
{
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::normalize( 0.3 * M_PI),  0.3 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::normalize(-0.6 * M_PI), -0.6 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::normalize( 2.9 * M_PI),  0.9 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::normalize(-2.9 * M_PI), -0.9 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::normalize( 1.5 * M_PI), -0.5 * M_PI);
}

void
AngleTest::testReverse()
{
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::reverse(-0.5 * M_PI),  0.5 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::reverse(-0.9 * M_PI),  0.1 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::reverse( 0.7 * M_PI), -0.3 * M_PI);
}

void
AngleTest::testPerpendicularCw()
{
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular(-0.5 * M_PI, true),  1.0 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular(-0.9 * M_PI, true),  0.6 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular( 0.7 * M_PI, true),  0.2 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular( 0.1 * M_PI, true), -0.4 * M_PI);
}

void
AngleTest::testPerpendicularCcw()
{
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular(-0.5 * M_PI, false),  0.0 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular(-0.9 * M_PI, false), -0.4 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular( 0.7 * M_PI, false), -0.8 * M_PI);
	TEST_ASSERT_EQUALS_FLOAT(xpcc::Angle::perpendicular( 0.1 * M_PI, false),  0.6 * M_PI);
}
