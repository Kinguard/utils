/*

	libutils - a collection of usable tools

	Copyright (c) 2015 Tor Krill <tor@openproducts.com>

	This file is part of libutils

	libutils is  free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "StateMachine.h"

namespace Utils {

StateMachine::StateMachine(): has_event( false )
{

}

void StateMachine::TriggerEvent( uint8_t new_state, Utils::EventData *event_data)
{
	if( new_state == StateMachine::EVENT_IGNORED ) // AKA do nothing
	{
		if( event_data != nullptr )
		{
			delete event_data;
		}
		return;
	}

	this->RegisterEvent( new_state, event_data );
	this->Process();
}

void StateMachine::RegisterEvent(uint8_t new_state, Utils::EventData *event_data)
{
	this->state = new_state;
	this->event_data = event_data;
	this->has_event = true;
}

void StateMachine::Process()
{
	ScopedLock l( this->m );
	EventData *tmpdata;

	while( this->has_event )
	{
		if( this->state == StateMachine::EVENT_ERROR )
		{
			throw std::runtime_error("State machine error");
		}

		if( this->statemap.find( this->state )== this->statemap.end() )
		{
			throw std::runtime_error("Illegal state in statemachine");
		}

		tmpdata = this->event_data;

		this->has_event = false;
		this->event_data = nullptr;

		this->statemap[this->state](tmpdata);

		if( tmpdata != nullptr )
		{
			delete tmpdata;
		}
	}
}

} // NS
