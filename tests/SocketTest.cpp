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
using Utils::MulticastSocket;

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

void SocketTest::testUDPSocketSendTo() {
	const char* group="239.255.255.250";
	uint16_t port = 1920;

	MulticastSocket rec;
	rec.Bind(port);
	rec.Join(group,"eth0");

	UDPSocket s;
	const char* buff="Im sent using sendto\r\n";
	s.SendTo(group,port,buff,strlen(buff));

	vector<char> data;
	rec.AppendTo(data,512);

	CPPUNIT_ASSERT_EQUAL(strlen(buff),data.size());
}


void SocketTest::testConnect()
{
	TCPClient t;

	CPPUNIT_ASSERT_NO_THROW(t.Connect(ECHO_SERVER,7));

	// Should not be able to reconnect(?)
	CPPUNIT_ASSERT_THROW(t.Connect(ECHO_SERVER,7),std::runtime_error);

	string hello("Hello\r\n");
	size_t wt = t.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);

	char buf[1024];
	size_t rd = t.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);

	string trd(buf,rd);
	CPPUNIT_ASSERT( hello == trd );

	UDPSocket s;
	CPPUNIT_ASSERT_NO_THROW(s.Connect(ECHO_SERVER,7));

	wt = s.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);

	rd = s.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);

	string tr2(buf,rd);
	CPPUNIT_ASSERT( hello == tr2 );

	// Should be able to reconnect an udp-socket
	CPPUNIT_ASSERT_NO_THROW(s.Connect(ECHO_SERVER,7));
}

void SocketTest::testBind()
{
	UDPSocket s;

	CPPUNIT_ASSERT_THROW(s.Bind(2233, "nonsens"),std::runtime_error);

	CPPUNIT_ASSERT_NO_THROW(s.Bind(2233, "eth0"));

	CPPUNIT_ASSERT_THROW(s.Bind(2233, "eth0"),std::runtime_error);

}

void SocketTest::testMulticast()
{
	const char* group="239.255.255.250";
	uint16_t port = 1900;

	MulticastSocket ms;

	CPPUNIT_ASSERT_EQUAL(1, ms.GetTTL());
	CPPUNIT_ASSERT_EQUAL(true, ms.GetLoopback() ) ;

	ms.Bind(port);
	CPPUNIT_ASSERT_THROW(ms.Join(group,"nonsens"),std::runtime_error);
	CPPUNIT_ASSERT_NO_THROW( ms.Join(group,"eth0") );

	CPPUNIT_ASSERT_NO_THROW( ms.Leave(group,"eth0") );
}

void SocketTest::testMulticastSend() {
	const char* group="239.255.255.250";
	uint16_t port = 1910;

	MulticastSocket rec;
	rec.Bind(port);
	rec.Join(group,"eth0");

	MulticastSocket send;
	send.Connect(group, port);
	const char* mess="Hello World!\r\n";
	send.Write(mess, strlen(mess));

	vector<char> data;
	rec.AppendTo(data,512);

	CPPUNIT_ASSERT_EQUAL(strlen(mess),data.size());

}








