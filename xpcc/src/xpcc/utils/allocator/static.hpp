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

#ifndef XPCC_ALLOCATOR__STATIC_HPP
#define XPCC_ALLOCATOR__STATIC_HPP

#include "allocator_base.hpp"

namespace xpcc
{
	namespace allocator
	{
		/**
		 * \brief	Static memory allocator
		 * 
		 * Allocates a big static block and distributes pieces of it during
		 * run-time. No reallocation is done when no more pieces are available.
		 * 
		 * \ingroup	allocator
		 * \author	Fabian Greif
		 */
		template <typename T,
				  std::size_t N>
		class Static : public AllocatorBase<T>
		{
		public:
			template <typename U>
			struct rebind
			{
				typedef Static<U, N> other;
			};
			
		public:
			Static() :
				AllocatorBase<T>()
			{
			}
			
			Static(const Static& other) :
				AllocatorBase<T>(other)
			{
			}
			
			template <typename U>
			Static(const Static<U, N>&) :
				AllocatorBase<T>()
			{
			}
			
			// TODO
			T*
			allocate(std::size_t other);
			
			void
			deallocate(T*);
			
		private:
			T memory[N];
		};
	}
}

#endif // XPCC_ALLOCATOR__STATIC_HPP
