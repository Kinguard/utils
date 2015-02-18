#include "StateMachine.h"

namespace Utils {

StateMachine::StateMachine(): has_event( false )
{

}

void StateMachine::TriggerEvent(const string &new_state, Utils::EventData *event_data)
{
	if( new_state == "" ) // AKA do nothing
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

void StateMachine::RegisterEvent(const string &new_state, Utils::EventData *event_data)
{
	this->state = new_state;
	this->event_data = event_data;
	this->has_event = true;
}

void StateMachine::Process()
{
	EventData *tmpdata;

	while( this->has_event )
	{
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
