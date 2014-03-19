// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2010, Roboterclub Aachen e.V.
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
/**
 * @file	syscalls.cpp
 * @brief	Stubs for newlib
 * 
 * Except for _sbrk_r() (which is needed for malloc()) all these stubs are
 * only dummy functions which don't do anything useful.
 * 
 * _exit() and exit() are defined in the file "startup.S". They are marked
 * as weak references, so they can be easily overwritten by user code.
 */
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>

#include <reent.h>
#include <sys/stat.h>

// ----------------------------------------------------------------------------
// __heap_start is set in the linker command file and is the end of
// statically allocated data (thus start of heap).

extern uint8_t __heap_start;
extern uint8_t __heap_end;
uint8_t *__brkval = &__heap_start;		// Points to current end of the heap

// ----------------------------------------------------------------------------
/*
 * @brief	Simple abort implementation
 * 
 * Does nothing except starting a infinite loop
 */ 
extern "C"
void
abort(void)
{
	while(1) {
		// do nothing
	}
}

// ----------------------------------------------------------------------------
extern "C"
int
_open_r(struct _reent */*r*/, const char */*name*/, int /*flags*/, int /*mode*/)
{
	return -1;
}

extern "C"
_ssize_t
_read_r(struct _reent */*r*/, int /*file*/, void */*ptr*/, size_t /*len*/)
{
	return 0;
}

extern "C"
_ssize_t
_write_r(struct _reent */*r*/, int /*file*/, const void */*ptr*/, size_t /*len*/)
{
	return 0;
}

extern "C"
int
_close_r(struct _reent */*r*/, int /*file*/)
{
	return 0;
}

// ----------------------------------------------------------------------------
extern "C"
_off_t
_lseek_r(struct _reent */*r*/, int /*file*/, _off_t /*ptr*/, int /*dir*/)
{
	return (_off_t) 0;		// Always indicate we are at file beginning.
}

// ----------------------------------------------------------------------------
extern "C"
int
_fstat_r(struct _reent */*r*/, int /*file*/, struct stat *st)
{
	st->st_mode = S_IFCHR;	// Always set as character device.
	return 0;
}

// ----------------------------------------------------------------------------
extern "C"
int
_isatty(int /*file*/)
{
	return 1;
}

// ----------------------------------------------------------------------------
/* Support function. Adjusts end of heap to provide more memory to
 * memory allocator. Simple and dumb with no sanity checks.
 * 
 *  struct _reent *r -- re-entrancy structure, used by newlib to
 *                      support multiple threads of operation.
 *  ptrdiff_t size   -- number of bytes to add.
 *                      Returns pointer to start of new heap area.
 *
 *  Note:  This implementation is not thread safe (despite taking a
 *         _reent structure as a parameter).
 *         Since the _reent struct is not used in the current implementation, 
 *         the following messages must be suppressed.
 */
extern "C"
void *
_sbrk_r(struct _reent *,  ptrdiff_t size)
{
	// move heap pointer
	uint8_t *heap = __brkval;
	__brkval += size;
	
	if (__brkval >= &__heap_end) {
		// ERROR: heap and stack collision!
		abort();
	}
	
	//  Return pointer to start of new heap area.
	return heap;
}
