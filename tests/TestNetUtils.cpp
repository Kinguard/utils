#include "TestNetUtils.h"

#include "NetUtils.h"
#include <iomanip>

CPPUNIT_TEST_SUITE_REGISTRATION (TestNetUtils);

using namespace std;

void TestNetUtils::setUp(){
}

void TestNetUtils::tearDown(){
}

void TestNetUtils::testGetHWAddr()
{
	CPPUNIT_ASSERT_THROW(Utils::Net::GetHWAddr("nonsens") , Utils::ErrnoException);

	vector<uint8_t> hw = Utils::Net::GetHWAddr("eth0");

	CPPUNIT_ASSERT_EQUAL(6, (int)hw.size());

#if 0
	for(auto x: hw){
		cout <<"["<< std::setw(2)<<std::setfill('0')<< std::hex << (int)x <<"]"<<endl;
	}
#endif
}

void TestNetUtils::testGetIfAddr() {
	CPPUNIT_ASSERT_THROW(Utils::Net::GetIfAddr("nonsens") , Utils::ErrnoException);
	CPPUNIT_ASSERT_NO_THROW(Utils::Net::GetIfAddr("eth0"));
}

void TestNetUtils::testGetInterfaces() {
	CPPUNIT_ASSERT_NO_THROW( Utils::Net::GetInterfaces() );
	list<string> ifs = Utils::Net::GetInterfaces();

	// Should have at least loopback interface
	CPPUNIT_ASSERT(ifs.size() > 0 );
#if 0
	for( auto x: ifs){
		cout << "Interface: "<<x<<endl;
	}
#endif
}
