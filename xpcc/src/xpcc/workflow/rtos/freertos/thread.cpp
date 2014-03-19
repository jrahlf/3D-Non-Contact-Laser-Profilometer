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

#include "../thread.hpp"

/*
 * FreeRTOS is a C library. Therefore it doesn't know anything about objects
 * and their this-pointers. To be able to use FreeRTOS from C++ we use a
 * simple trick:
 *
 * FreeRTOS threads must be C functions which get a single void-Pointer as
 * argument. To call C++ member functions we use the static member function
 * xpcc::rtos::Thread::wrapper() as a wrapper which gets the a pointer to
 * the corresponding thread as parameter.
 * To be able to call the right run()-function run() needs to be virtual. This
 * adds the overhead of an additional vtable per Thread but is the only portable
 * solution.
 * Casting member functions pointer to void-pointers and back might work but
 * is not guaranteed to.
 */

// ----------------------------------------------------------------------------
void
xpcc::rtos::Thread::wrapper(void *object)
{
	Thread* thread = reinterpret_cast<Thread *>(object);
	thread->run();
}

xpcc::rtos::Thread::~Thread()
{
	vTaskDelete(this->handle);
}

// ----------------------------------------------------------------------------
xpcc::rtos::Thread::Thread(uint32_t priority,
		uint16_t stackDepth,
		const char* name)
{
	xTaskCreate(
			&wrapper,
			(const signed char*) name,
			(stackDepth / 4) + 1,
			this,
			priority,
			&this->handle);
}

// ----------------------------------------------------------------------------
uint32_t
xpcc::rtos::Thread::getPriority() const
{
	return uxTaskPriorityGet(this->handle);
}

void
xpcc::rtos::Thread::setPriority(uint32_t priority)
{
	vTaskPrioritySet(this->handle, priority);
}

// ----------------------------------------------------------------------------
/*void
xpcc::rtos::Thread::suspend()
{
	vTaskSuspend(this->handle);
}

void
xpcc::rtos::Thread::resume()
{
	vTaskResume(this->handle);
}

void
xpcc::rtos::Thread::resumeFromInterrupt()
{
	xTaskResumeFromISR(this->handle);
}*/

