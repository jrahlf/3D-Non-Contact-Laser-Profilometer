// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
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

#include "../semaphore.hpp"

// ----------------------------------------------------------------------------
xpcc::rtos::Semaphore::Semaphore(uint32_t max, uint32_t initial) :
	count(initial), maxCount(max)
{
}

// ----------------------------------------------------------------------------
bool
xpcc::rtos::Semaphore::acquire(uint32_t timeout)
{
	boost::unique_lock<boost::mutex> lock(mutex);
	while (count == 0)
	{
		 if (!condition.timed_wait(lock,
				 boost::posix_time::milliseconds(timeout))) {
			 return false;
		 }
	}
	--count;
	
	return true;
}

void
xpcc::rtos::Semaphore::release()
{
	boost::unique_lock<boost::mutex> lock(mutex);
	
	if (count < maxCount) {
		++count;
		
		// Wake up any waiting threads. 
		// Always do this, even if count_ wasn't 0 on entry. Otherwise, we
		// might not wake up enough waiting threads if we get a number of
		// signal() calls in a row.
		condition.notify_one();
	}
}

// ----------------------------------------------------------------------------
xpcc::rtos::BinarySemaphore::BinarySemaphore() :
		Semaphore(1, 1)
{
}
