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

#ifndef	XPCC_SAB__SLAVE_HPP
	#error	"Don't include this file directly, use 'slave.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template <typename T>
void
xpcc::sab::Response::send(const T& payload)
{
	triggered = true;
	transmitter->send(true, reinterpret_cast<const void *>(&payload), sizeof(T));
}

// ----------------------------------------------------------------------------
inline void
xpcc::sab::Action::call(Response& response, const void *payload)
{
	// redirect call to the actual object
	(object->*function)(response, payload);
}

// Disable warnings for Visual Studio about using 'this' in a base member
// initializer list.
// In this case though it is totally safe so it is ok to disable this warning.
#ifdef XPCC__COMPILER_MSVC
#	pragma warning(disable:4355)
#endif

// ----------------------------------------------------------------------------
template <typename Interface>
xpcc::sab::Slave<Interface>::Slave(uint8_t address,
		xpcc::accessor::Flash<Action> list,
		uint8_t count) : 
	ownAddress(address), actionList(list), actionCount(count),
	response(this)
{
	Interface::initialize();
}			

// ----------------------------------------------------------------------------
template <typename Interface>
void
xpcc::sab::Slave<Interface>::update()
{
	Interface::update();
	if (Interface::isMessageAvailable())
	{
		if (Interface::getAddress() == this->ownAddress &&
				!Interface::isResponse())
		{
			this->response.triggered = false;
			this->currentCommand = Interface::getCommand();
			
			xpcc::accessor::Flash<Action> list = actionList;
			for (uint_fast8_t i = 0; i < actionCount; ++i, ++list)
			{
				Action action(*list);
				if (this->currentCommand == action.command)
				{
					if (Interface::getPayloadLength() == action.payloadLength)
					{
						// execute callback function
						action.call(this->response, Interface::getPayload());
						
						if (!this->response.triggered) {
							this->response.error(ERROR__NO_RESPONSE);
						}
					}
					else {
						this->response.error(ERROR__WRONG_PAYLOAD_LENGTH);
					}
					break;
				}
			}
			
			if (!this->response.triggered) {
				this->response.error(ERROR__NO_ACTION);
			}
		}
		Interface::dropMessage();
	}
}

// ----------------------------------------------------------------------------
template <typename Interface>
void
xpcc::sab::Slave<Interface>::send(bool acknowledge,
			const void *payload, std::size_t payloadLength)
{
	Flags flags;
	if (acknowledge) {
		flags = xpcc::sab::ACK;
	}
	else {
		flags = xpcc::sab::NACK;
	}
	
	Interface::sendMessage(this->ownAddress, flags, this->currentCommand,
			payload, payloadLength);
}
