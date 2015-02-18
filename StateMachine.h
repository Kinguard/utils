#ifndef STATEMACHINE_H
#define STATEMACHINE_H


#include <functional>
#include <map>

using namespace std;

namespace Utils
{

class EventData
{
public:
	virtual ~EventData(){}
};

typedef std::function<void(EventData*)> StateFunction;
typedef map<string, StateFunction> StateMap;

/*
 * Very lightweight, featurewise, statemachine.
 *
 * To use, derive class, fill statemap and implement external events
 */
class StateMachine
{
public:
	StateMachine(const StateMachine&) = delete;
	StateMachine& operator=(const StateMachine&) = delete;

	StateMachine();

	virtual ~StateMachine() {}

protected:
	// Use in external event functions to trigger statemachine
	void TriggerEvent( const string& new_state, EventData* event_data );

	// Use internally to transition to new state.
	void RegisterEvent( const string& new_state, EventData* event_data);

	void Process();

	string state;
	EventData* event_data;
	bool has_event;
	StateMap statemap;
};

} // NS

#endif // STATEMACHINE_H
