#include "TestStateMachine.h"

#include "StateMachine.h"
using namespace Utils;

CPPUNIT_TEST_SUITE_REGISTRATION ( TestStateMachine );

class SMTest: public Utils::StateMachine
{
public:
	enum
	{
		STATE1,
		STATE2,
		STATE3
	};
	SMTest(): StateMachine()
	{
		this->statemap =
		{
			{STATE1, std::bind( &SMTest::State1, this, std::placeholders::_1 )},
			{STATE2, std::bind( &SMTest::State2, this, std::placeholders::_1 )},
			{STATE3, std::bind( &SMTest::State3, this, std::placeholders::_1 )},
		};
		this->state = STATE2;
	}

	void S1()
	{
		this->TriggerEvent(STATE1, nullptr);
	}

	void S2()
	{
		this->TriggerEvent(STATE2, nullptr);
	}

	uint8_t State(){ return this->state;}

	virtual ~SMTest(){}
protected:
	void State1(EventData* data) {}

	void State2(EventData* data)
	{
		this->RegisterEvent(STATE3, nullptr);
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
	CPPUNIT_ASSERT( t.State() == SMTest::STATE2 );
	CPPUNIT_ASSERT_NO_THROW( t.S1() );
	CPPUNIT_ASSERT( t.State() == SMTest::STATE1 );
	CPPUNIT_ASSERT_NO_THROW( t.S1() );
	CPPUNIT_ASSERT( t.State() == SMTest::STATE1 );
	CPPUNIT_ASSERT_NO_THROW( t.S2() );
	CPPUNIT_ASSERT( t.State() == SMTest::STATE3 );

}
