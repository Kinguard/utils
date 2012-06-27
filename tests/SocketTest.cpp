/*
 * SocketTest.cpp
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#include "SocketTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SocketTest);

#include "Socket.h"

using Utils::Socket;
using Utils::TCPClient;
using Utils::UDPSocket;

#include <iostream>

using namespace std;

#define ECHO_SERVER "tor-desktop"


void SocketTest::setUp() {
}

void SocketTest::tearDown() {
}

void SocketTest::testSocketConstructor() {
	Socket s;
	CPPUNIT_ASSERT(s.getSocketFd() == -1);

	CPPUNIT_ASSERT_THROW( Socket s(-1,-1), Utils::ErrnoException);

	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::Local, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::Local, Socket::Datagram) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV4, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV4, Socket::Datagram) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV6, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV6, Socket::Datagram) );
}

void SocketTest::testTCPClientConstructor()
{
	TCPClient s;
	// Should have a valid socket
	CPPUNIT_ASSERT( s.getSocketFd() > -1);

	CPPUNIT_ASSERT_NO_THROW(TCPClient t() );
	CPPUNIT_ASSERT_NO_THROW(TCPClient t("www.google.se",80) );
	//TODO: fix error cases not possible atm due to opendns stupid behavior
}

void SocketTest::testTCPClientEcho() {
	TCPClient t(ECHO_SERVER, 7);
	string hello("Hello\r\n");
	char buf[1024];

	// Buffer implementation
	size_t wt = t.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);
	size_t rd = t.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);
	string trd(buf,rd);
	CPPUNIT_ASSERT( hello == trd );

	//cerr << "Sent ["<<hello<<"] got ["<<trd<<"]"<<endl;

	// Vector implementation
	vector<char> v;
	v.reserve(1024);
	string goodbye("Good Bye!\r\n");
	vector<char> line(goodbye.begin(),goodbye.end());

	wt = t.Write(line);
	CPPUNIT_ASSERT_EQUAL(goodbye.length(), wt);
	rd = t.Read(v);
	CPPUNIT_ASSERT_EQUAL(wt,rd);
	string trd2(&v[0],rd);
	CPPUNIT_ASSERT( goodbye == trd2 );
}

void SocketTest::testUDPSocketConstructor()
{
	UDPSocket s;

	// Should have a valid socket
	CPPUNIT_ASSERT(s.getSocketFd() > -1);
	CPPUNIT_ASSERT_NO_THROW(UDPSocket s() );
	CPPUNIT_ASSERT_NO_THROW(UDPSocket s(ECHO_SERVER, 7) );

}

void SocketTest::testUDPSocketEcho()
{
	UDPSocket s(ECHO_SERVER, 7);
	string hello("Hello\r\n");
	char buf[1024];

	size_t wt = s.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);
	size_t rd = s.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);
	string trd(buf,rd);
	CPPUNIT_ASSERT( hello == trd );
	//cerr << "Sent ["<<hello<<"] got ["<<trd<<"]"<<endl;
}




