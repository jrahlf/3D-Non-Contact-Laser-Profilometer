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

#ifndef XPCC__TIMEOUT_HPP
#define XPCC__TIMEOUT_HPP

#include <xpcc/architecture/driver/clock.hpp>

#include "timestamp.hpp"

namespace xpcc
{
	/**
	 * \brief		Software timer
	 *
	 * Has to be polled to check if it has expired.
	 * 
	 * Extra care must be taken when not calling the isExpired() method
	 * for more than ~30000 ticks (which corresponds to about 30s with the
	 * default millisecond resolution) when the timeout is active. Due to an
	 * overflow in the implementation this might add an additional delay of
	 * up to ~30000 ticks in the worst case.
	 * 
	 * Call isExpired() at least once every 30000 ticks before the timeout
	 * value is reached to avoid this behaviour.
	 * 
	 * \see		PeriodicTimer
	 * \tparam	T	Used timer, default is xpcc::Clock() which has 
	 * 				a millisecond resolution.
	 * 
	 * Usage:
	 * \include	timeout.cpp
	 * 
	 * Complete example for the ATMega644:
	 * \include timeout/main.cpp
	 * 
	 * \author	Fabian Greif
	 * \ingroup	workflow
	 */
	template<typename T = ::xpcc::Clock>
	class Timeout
	{
	public:
		Timeout();
		
		/**
		 * \brief	Create and start the timeout
		 */
		Timeout(const Timestamp time);
		
		/**
		 * \brief	Check if the given time has passed.
		 * 
		 * If isExpired() changes to \c true, it will keep this value till
		 * a call of restart().
		 */
		bool
		isExpired();
		
		/**
		 * \brief	Check if the timer was started and was not stopped.
		 *
		 * If isActive() changes to \c false, it will keep this value till
		 * a call of restart().
		 */
		inline bool
		isActive() const;

		/**
		 * \brief	Stop the timer
		 * 
		 * Sets isExpired() to \c true, and isActive() to \c false.
		 */
		inline void
		stop();
		
		/**
		 * \brief	Set a new timeout value.
		 */
		void
		restart(Timestamp time);
		
		/**
		 * \brief	Returns the remaining time until a timeout.
		 */
		inline xpcc::Timestamp
		remaining();

	private:
		enum State
		{
			STOPPED,
			ACTIVE,
			EXPIRED
		};
		
		Timestamp endTime;
		State state;
	};
}

#include "timeout_impl.hpp"

#endif // XPCC__TIMEOUT_HPP
