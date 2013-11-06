#ifndef TESTUSERGROUPS_H_
#define TESTUSERGROUPS_H_

#include <cppunit/extensions/HelperMacros.h>

class TestUserGroups: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestUserGroups );
	CPPUNIT_TEST( TestUser );
	CPPUNIT_TEST( TestGroup );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void TestUser();
	void TestGroup();
};

#endif /* TESTUSERGROUPS_H_ */
