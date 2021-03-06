/*
 * SocketTest.h
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#ifndef SOCKETTEST_H_
#define SOCKETTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include <string>

class SocketTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SocketTest );
	CPPUNIT_TEST( testSocketConstructor );
	CPPUNIT_TEST( testTCPClientConstructor );
	CPPUNIT_TEST( testTCPClientEcho );
	CPPUNIT_TEST( testTCPServer );
	CPPUNIT_TEST( testUDPSocketConstructor );
	//CPPUNIT_TEST( testUDPSocketEcho );
	CPPUNIT_TEST( testUDPSocketSendTo );
	CPPUNIT_TEST( testConnect );
	CPPUNIT_TEST( testBind );
	CPPUNIT_TEST( testMulticast );
	CPPUNIT_TEST( testMulticastSend );
	CPPUNIT_TEST( testUnixStreamSocket );
	CPPUNIT_TEST( testUnixFdPass );
	CPPUNIT_TEST_SUITE_END();
public:
	SocketTest();
	void setUp();
	void tearDown();

	void testSocketConstructor();

	void testTCPClientConstructor();
	void testTCPClientEcho();

	void testTCPServer();


	void testUDPSocketConstructor();
	void testUDPSocketEcho();
	void testUDPSocketSendTo();
	void testConnect();
	void testBind();

	void testMulticast();
	void testMulticastSend();

	void testUnixStreamSocket();
	void testUnixFdPass();
private:
	std::string netif;
};

#endif /* SOCKETTEST_H_ */
