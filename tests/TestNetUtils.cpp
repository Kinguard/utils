#include "TestNetUtils.h"

#include "NetUtils.h"
#include <iomanip>
#include <algorithm>

CPPUNIT_TEST_SUITE_REGISTRATION (TestNetUtils);

using namespace std;

void TestNetUtils::setUp(){
}

void TestNetUtils::tearDown(){
}

static string GetIfName()
{
	// Get first ifname not being localnet
	string iface = "eth0";

	list<string> ifs = Utils::Net::GetInterfaces();

	auto it = std::find_if_not( ifs.begin(), ifs.end(), [](const string& n){ return n=="lo";} );

	if ( it != ifs.end() )
	{
		iface = *it;
	}

	return iface;
}

void TestNetUtils::testGetHWAddr()
{
	CPPUNIT_ASSERT_THROW(Utils::Net::GetHWAddr("nonsens") , Utils::ErrnoException);

	vector<uint8_t> hw = Utils::Net::GetHWAddr(GetIfName());

	CPPUNIT_ASSERT_EQUAL(6, (int)hw.size());

#if 0
	for(auto x: hw){
		cout <<"["<< std::setw(2)<<std::setfill('0')<< std::hex << (int)x <<"]"<<endl;
	}
#endif
}

void TestNetUtils::testGetNetmask()
{
	struct sockaddr addr {};

	addr = Utils::Net::GetNetmask(GetIfName());
	CPPUNIT_ASSERT( Utils::Net::SockAddrToString(&addr) != "");
}

void TestNetUtils::testGetIfAddr() {
	CPPUNIT_ASSERT_THROW(Utils::Net::GetIfAddr("nonsens") , Utils::ErrnoException);

	CPPUNIT_ASSERT_NO_THROW(Utils::Net::GetIfAddr(GetIfName()));
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
