#include "TestRegex.h"

#include "Regex.h"

using namespace Utils;

CPPUNIT_TEST_SUITE_REGISTRATION ( TestRegex );

void TestRegex::setUp()
{
}

void TestRegex::tearDown()
{
}

void TestRegex::Test()
{
	Regex r;
	CPPUNIT_ASSERT_THROW( r.DoMatch("Hepp"),runtime_error);
	CPPUNIT_ASSERT_NO_THROW( r.Compile("Hepp") );
	CPPUNIT_ASSERT_NO_THROW( r.Compile("Hiopp") );

	vector<Regex::Match> m;
	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("nothing"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 0, m.size());

	CPPUNIT_ASSERT_NO_THROW( r.Compile("H[io]pp") );

	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("nothing"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 0, m.size());

	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("Hopp"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 1, m.size());

	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("Hipp"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 1, m.size());

	CPPUNIT_ASSERT_NO_THROW( r.Compile("([0-9]{2})\\s+(\\S+)\\s+(.+)\\s*;") );

	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("nothing"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 0, m.size());

	CPPUNIT_ASSERT_NO_THROW( m = r.DoMatch("10 print 'hello world';"));
	CPPUNIT_ASSERT_EQUAL( (size_t) 4, m.size());
}
