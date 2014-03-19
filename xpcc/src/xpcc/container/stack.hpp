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

#ifndef	XPCC__STACK_HPP
#define	XPCC__STACK_HPP

#include <cstddef>
#include <stdint.h>

#include "deque.hpp"

namespace xpcc
{
	/**
	 * \brief	LIFO stack
	 * 
	 * Elements are pushed/popped from the "back" of the specific container,
	 * which is known as the top of the stack.
	 * 
	 * getFront(), removeFront(), prepend()
	 * 
	 * \see		Deque()
	 * 
	 * \author	Fabian Greif 
	 * \ingroup		container
	 */
	template<typename T,
			 typename Container>
	class Stack
	{
	public:
		typedef typename Container::Size Size;
		
	public:
		bool
		isEmpty()
		{
			return c.isEmpty();
		}
		
		bool
		isFull()
		{
			return c.isFull();
		}
		
		Size
		getSize()
		{
			return c.getSize();
		}
		
		Size
		getMaxSize()
		{
			return c.getMaxSize();
		}
		
		T&
		get()
		{
			return c.getFront();
		}
		
		const T&
		get() const
		{
			return c.getFront();
		}
		
		bool
		push(const T& value)
		{
			return c.prepend(value);
		}
		
		void
		pop()
		{
			c.removeFront();
		}
	
	protected:
		Container c;
	};

	// ------------------------------------------------------------------------
	/**
	 * \brief		Bounded stack
	 * 
	 * \ingroup		container
	 */
	template<typename T, 
			 std::size_t N,
			 typename Container = BoundedDeque<T, N> >
	class BoundedStack : public Stack<T, Container>
	{
	};
}

#endif	// XPCC__STACK_HPP
