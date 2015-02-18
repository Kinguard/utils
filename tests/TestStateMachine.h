#ifndef TESTSTATEMACHINE_H_
#define TESTSTATEMACHINE_H_

#include <cppunit/extensions/HelperMacros.h>

class TestStateMachine: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestStateMachine );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void Test();
};

#endif /* TESTSTATEMACHINE_H_ */