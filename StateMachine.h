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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <functional>
#include <map>

#include "Mutex.h"

using namespace std;

namespace Utils
{

class EventData
{
public:
	virtual ~EventData(){}
};

typedef std::function<void(EventData*)> StateFunction;
typedef map<uint8_t, StateFunction> StateMap;

/*
 * Very lightweight, featurewise, statemachine.
 *
 * To use, derive class, fill statemap and implement external events
 */
class StateMachine
{
public:
	enum {
		EVENT_IGNORED = 0xfe,
		EVENT_ERROR
	};
	StateMachine(const StateMachine&) = delete;
	StateMachine& operator=(const StateMachine&) = delete;

	StateMachine();

	virtual ~StateMachine() {}

protected:
	// Use in external event functions to trigger statemachine
	void TriggerEvent( uint8_t new_state, EventData* event_data );

	// Use internally to transition to new state.
	void RegisterEvent( uint8_t new_state, EventData* event_data);

	void Process();

	uint8_t state;
	EventData* event_data;
	bool has_event;
	StateMap statemap;
	Mutex m;
};

} // NS

#endif // STATEMACHINE_H
