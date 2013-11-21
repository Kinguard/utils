#ifndef TESTARGPARSE_H_
#define TESTARGPARSE_H_

#include <cppunit/extensions/HelperMacros.h>

class TestArgParse: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestArgParse ); 
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	 
	void Test();
};

#endif /* TESTARGPARSE_H_ */