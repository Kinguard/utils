#include "TestSimpleHttpClient.h"

#include "SimpleHttpClient.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestSimpleHttpClient);

using namespace Utils;

void TestSimpleHttpClient::setUp(){
}

void TestSimpleHttpClient::tearDown(){
}

void TestSimpleHttpClient::test()
{
	SimpleHttpClient c;

	CPPUNIT_ASSERT_EQUAL(string(""),c.getHost());
	CPPUNIT_ASSERT_EQUAL(string(""),c.getPath());
	CPPUNIT_ASSERT_EQUAL(0,c.getPort());

	CPPUNIT_ASSERT_THROW(c.Fetch(),std::runtime_error);

	c.setPort(80);
	CPPUNIT_ASSERT_THROW(c.Fetch(),std::runtime_error);

	c.setHost("www.google.se");
	CPPUNIT_ASSERT_THROW(c.Fetch(),std::runtime_error);

	c.setPath("/");
	CPPUNIT_ASSERT_NO_THROW(c.Fetch());
	CPPUNIT_ASSERT_EQUAL(string("gws"),c.Header("SERVER"));

	vector<char> body = c.Content();
	CPPUNIT_ASSERT(body.size()>0);

}
