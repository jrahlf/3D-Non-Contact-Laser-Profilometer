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

#include "../dynamic_postman.hpp"

// ----------------------------------------------------------------------------
xpcc::DynamicPostman::DynamicPostman() :
	eventMap(0),
	requenstMap(0)
{
}

// ----------------------------------------------------------------------------
xpcc::DynamicPostman::DynamicPostman(const EventMap* eventMap, const RequestMap* requenstMap) :
	eventMap(eventMap),
	requenstMap(requenstMap)
{
}

// ----------------------------------------------------------------------------
xpcc::DynamicPostman::DeliverInfo
xpcc::DynamicPostman::deliverPacket(const Header &header, const SmartPointer& payload)
{
	if (this->eventMap != 0)
	{
		if (header.destination == 0)
		{
			// EVENT
			EventMap::const_iterator lowerBound(this->eventMap->lower_bound(header.packetIdentifier));
			EventMap::const_iterator upperBound(this->eventMap->upper_bound(header.packetIdentifier));
			if (lowerBound != upperBound) {
				do {
					lowerBound->second.call(header, payload);
					lowerBound++;
				}
				while (lowerBound != upperBound);
				return OK;
			}
			return NO_EVENT;
		}
		else
		{
			// REQUEST
			RequestMap::const_iterator iterDestination(this->requenstMap->find(header.destination));
			if (iterDestination != this->requenstMap->end())
			{
				CallbackMap::const_iterator iterCallback(iterDestination->second.find(header.packetIdentifier));
				if (iterCallback != iterDestination->second.end())
				{
					ResponseHandle response(header);
					iterCallback->second.call(response, payload);
					return OK;
				}
				else {
					return NO_ACTION;
				}
			}
			else {
				return NO_COMPONENT;
			}
		}
	}
	else {
		return NO_COMPONENT;
	}
}

// ----------------------------------------------------------------------------

bool
xpcc::DynamicPostman::isComponentAvaliable(uint8_t component) const
{
	if (this->requenstMap != 0) {
		return (this->requenstMap->find(component) != this->requenstMap->end());
	}
	else {
		return false;
	}
}
