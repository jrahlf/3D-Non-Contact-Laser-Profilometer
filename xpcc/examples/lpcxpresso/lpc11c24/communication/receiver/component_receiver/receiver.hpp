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

#ifndef COMPONENT__RECEIVER_HPP
#define COMPONENT__RECEIVER_HPP

#include <xpcc/architecture.hpp>
#include <xpcc/communication/abstract_component.hpp>
#include <xpcc/workflow/periodic_timer.hpp>

#include "communication/packets.hpp"

#include <xpcc/driver/ui/display/siemens_s65.hpp>

namespace lcd
{
	GPIO__OUTPUT(Rs,    2,  1);
	GPIO__OUTPUT(Reset, 2,  0);
	GPIO__OUTPUT(Cs,    2,  2);
}
typedef xpcc::lpc::SpiMaster0 SpiDisplay;
typedef xpcc::SiemensS65Landscape<SpiDisplay, lcd::Cs, lcd::Rs, lcd::Reset> Display;

namespace component
{
	class Receiver : public xpcc::AbstractComponent
	{
	public:
		Receiver(uint8_t id, xpcc::Dispatcher *communication);
		
		void
		initialize();

		void
		eventPosition(
				const xpcc::Header & header,
				const robot::packet::Position * parameter);

		void
		actionSetPosition(const xpcc::ResponseHandle& handle,
				const robot::packet::Position *parameter);
		
		void
		actionGetPosition(const xpcc::ResponseHandle& handle);
		
		void
		update();
		
	private:
		robot::packet::Position position;
		xpcc::PeriodicTimer<> displayTimer;
	};
}

#endif // COMPONENT__RECEIVER_HPP
