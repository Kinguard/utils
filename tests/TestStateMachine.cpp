#include "TestStateMachine.h"

#include "StateMachine.h"
using namespace Utils;

CPPUNIT_TEST_SUITE_REGISTRATION ( TestStateMachine );

class SMTest: public Utils::StateMachine
{
public:
	SMTest(): StateMachine()
	{
		this->statemap =
		{
			{"state1", std::bind( &SMTest::State1, this, std::placeholders::_1 )},
			{"state2", std::bind( &SMTest::State2, this, std::placeholders::_1 )},
			{"state3", std::bind( &SMTest::State3, this, std::placeholders::_1 )},
		};
		this->state = "state2";
	}

	void S1()
	{
		this->TriggerEvent("state1", nullptr);
	}

	void S2()
	{
		this->TriggerEvent("state2", nullptr);
	}

	string State(){ return this->state;}

	virtual ~SMTest(){}
protected:
	void State1(EventData* data) {}

	void State2(EventData* data)
	{
		this->RegisterEvent("state3", nullptr);
	}

	void State3(EventData* data){}
};


void TestStateMachine::setUp()
{
}

void TestStateMachine::tearDown()
{
}

void TestStateMachine::Test()
{
	SMTest t;
	CPPUNIT_ASSERT( t.State() == "state2" );
	CPPUNIT_ASSERT_NO_THROW( t.S1() );
	CPPUNIT_ASSERT( t.State() == "state1" );
	CPPUNIT_ASSERT_NO_THROW( t.S1() );
	CPPUNIT_ASSERT( t.State() == "state1" );
	CPPUNIT_ASSERT_NO_THROW( t.S2() );
	CPPUNIT_ASSERT( t.State() == "state3" );

}
