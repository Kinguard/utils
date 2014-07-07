/*
 * SocketTest.cpp
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#include "SocketTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SocketTest);

#include "Socket.h"
#include "NetUtils.h"
#include "Config.h"

using Utils::Net::Socket;
using Utils::Net::TCPClientSocket;
using Utils::Net::TCPServerSocket;
using Utils::Net::UDPClientSocket;
using Utils::Net::UDPServerSocket;
using Utils::Net::MulticastSocket;
using Utils::Net::UnixStreamClientSocket;
using Utils::Net::UnixStreamServerSocket;

#include <iostream>

using namespace std;

#define ECHO_SERVER "tor-desktop"

#include "Thread.h"
#include "Condition.h"


class TestServer: public Utils::Thread {
protected:
	Utils::Condition joined;
public:
	TestServer(bool detached): Thread(detached)
	{

	}

	void WaitJoined(void)
	{
		this->joined.Wait();
	}


	virtual ~TestServer(){}
};


void SocketTest::setUp() {
}

void SocketTest::tearDown() {
}

void SocketTest::testSocketConstructor() {

	CPPUNIT_ASSERT_THROW( Socket s(Socket::NoDomain,Socket::NoType), Utils::ErrnoException);

	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::Local, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::Local, Socket::Datagram) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV4, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV4, Socket::Datagram) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV6, Socket::Stream) );
	CPPUNIT_ASSERT_NO_THROW( Socket s(Socket::IPV6, Socket::Datagram) );
}

void SocketTest::testTCPClientConstructor()
{

	CPPUNIT_ASSERT_NO_THROW(TCPClientSocket t() );
	CPPUNIT_ASSERT_NO_THROW(TCPClientSocket t("www.google.se",80) );
	//TODO: fix error cases not possible atm due to opendns stupid behavior
}

void SocketTest::testTCPClientEcho() {
	TCPClientSocket t(ECHO_SERVER, 7);
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
	CPPUNIT_ASSERT_NO_THROW(UDPClientSocket s() );
	CPPUNIT_ASSERT_NO_THROW(UDPClientSocket s(ECHO_SERVER, 7) );
}

void SocketTest::testUDPSocketEcho()
{
	UDPClientSocket s(ECHO_SERVER, 7);
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

class Listen: public TestServer {
private:
	vector<char> data;
public:
	Listen(bool detached): TestServer(detached)
	{
	}

	size_t size()
	{
		return this->data.size();
	}

	virtual void Run()
	{
		const char* group="239.255.255.250";
		MulticastSocket rec("1920");

		rec.Join(group,"eth0");
		this->joined.Notify();
		rec.AppendTo(data,512);
	}
};

#include <unistd.h>
void SocketTest::testUDPSocketSendTo() {
	const char* group="239.255.255.250";
	uint16_t port = 1920;

	Listen l(false);

	l.Start();
	l.WaitJoined();
	UDPClientSocket s;
	const char* buff="Im sent using sendto\r\n";
	s.SendTo(group,port,buff,strlen(buff));

	l.Join();


	CPPUNIT_ASSERT_EQUAL(strlen(buff),l.size());

}


void SocketTest::testConnect()
{
	TCPClientSocket t(ECHO_SERVER,7);

	string hello("Hello\r\n");
	size_t wt = t.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);

	char buf[1024];
	size_t rd = t.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);

	string trd(buf,rd);
	CPPUNIT_ASSERT( hello == trd );

	UDPClientSocket s(ECHO_SERVER,7);

	wt = s.Write(hello.c_str(),hello.length());
	CPPUNIT_ASSERT_EQUAL(hello.length(), wt);

	rd = s.Read(buf,1024);
	CPPUNIT_ASSERT_EQUAL(wt,rd);

	string tr2(buf,rd);
	CPPUNIT_ASSERT( hello == tr2 );
}

void SocketTest::testBind()
{

	CPPUNIT_ASSERT_THROW( UDPServerSocket("nonsens","2233"),std::runtime_error);

	CPPUNIT_ASSERT_NO_THROW( UDPServerSocket("eth0","2233") );

}

void SocketTest::testMulticast()
{
	const char* group="239.255.255.250";

	MulticastSocket ms("1900");

	CPPUNIT_ASSERT_EQUAL(1, ms.GetTTL());
	CPPUNIT_ASSERT_EQUAL(true, ms.GetLoopback() ) ;

	CPPUNIT_ASSERT_THROW(ms.Join(group,"nonsens"),std::runtime_error);
	CPPUNIT_ASSERT_NO_THROW( ms.Join(group,"eth0") );

	CPPUNIT_ASSERT_NO_THROW( ms.Leave(group,"eth0") );
}

void SocketTest::testMulticastSend() {
	const char* group="239.255.255.250";
	uint16_t port = 1910;

	MulticastSocket rec("1910");
	rec.Join(group,"eth0");

	UDPClientSocket send(group, port);
	const char* mess="Hello World!\r\n";
	send.Write(mess, strlen(mess));

	vector<char> data;
	rec.AppendTo(data,512);

	CPPUNIT_ASSERT_EQUAL(strlen(mess),data.size());

}

class TcpServer: public TestServer {
private:
	TCPServerSocket s;
public:

	TcpServer():TestServer(true),s("eth0",2233){}

	virtual void Run()
	{
		Utils::Net::SocketPtr c = s.Accept();
		if( c )
		{
			vector<char> data;
			c->AppendTo(data,512);

			c->Write(data);
		}else{
			throw std::runtime_error("Accept failed");
		}

	}
};


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void
SocketTest::testTCPServer ()
{
	TcpServer s;

	s.Start();
	usleep(1000); // Give server time to start up

	struct sockaddr addr=Utils::Net::GetIfAddr("eth0");
	string ethaddr(inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr));

	TCPClientSocket c(ethaddr, 2233);
	const char buf[] = "Hello World!";
	c.Write( (void*) buf, strlen(buf ));

	char rep[100];
	size_t rd = c.Read(rep, sizeof(rep));
	rep[rd]=0;
	CPPUNIT_ASSERT_EQUAL(strlen(buf), strlen(rep));
	string s1(buf),s2(rep);
	CPPUNIT_ASSERT( s1 == s2 );

}


class UnixServer: public TestServer {
private:
	UnixStreamServerSocket s;
public:

	UnixServer():TestServer(false),s("/tmp/socktest")
	{
	}

	virtual void Run()
	{
		this->joined.Notify();
		Utils::Net::SocketPtr c = s.Accept();
		if( c )
		{
			vector<char> data;
			c->AppendTo(data,512);

			c->Write(data);
		}
		else
		{
			throw std::runtime_error("Accept failed");
		}
	}
};


void
SocketTest::testUnixStreamSocket()
{
	CPPUNIT_ASSERT_THROW(UnixStreamServerSocket("/bla/kjkj") ,Utils::ErrnoException);

	UnixServer s;

	s.Start();
	s.WaitJoined();
	try{
		UnixStreamClientSocket c("/tmp/socktest");

		const char buf[] = "Hello World!";
		size_t wt = c.Write( (void*) buf, strlen(buf ));

		char rep[100];
		size_t rd = c.Read(rep, sizeof(rep));
		rep[rd] = '\0';

		CPPUNIT_ASSERT_EQUAL(wt, rd);

		CPPUNIT_ASSERT_EQUAL(strlen(buf), strlen(rep));
		string s1(buf),s2(rep);
		CPPUNIT_ASSERT( s1 == s2 );

		// In process should yield our ids
		struct ucred uc = c.GetCredentials();
		CPPUNIT_ASSERT_EQUAL( getuid(), uc.uid );
		CPPUNIT_ASSERT_EQUAL( getgid(), uc.gid );
		CPPUNIT_ASSERT_EQUAL( getpid(), uc.pid );


	}catch(Utils::ErrnoException& e)
	{
		std::cerr << "Failed to test socket "<<e.what()<<std::endl;
	}
}
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class UnixFDTest: public TestServer {
private:
	UnixStreamServerSocket s;
public:

	UnixFDTest():TestServer(false),s("/tmp/socktest"){}

	virtual void Run()
	{
		this->joined.Notify();
		s.SetTimeout(1,0);
		Utils::Net::SocketPtr c = s.Accept();
		if( c )
		{
			int fd = open(TESTUTILS_PATH"/testfil.txt",O_RDONLY);
			if( fd <0 )
			{
				throw std::runtime_error("Failed to open test file");
			}

			UnixStreamClientSocket *cs=static_cast<UnixStreamClientSocket *>(c.get());

			cs->SendFd(fd);
			close(fd);
		}
		else
		{
			throw std::runtime_error("Accept failed");
		}
	}
};


void
SocketTest::testUnixFdPass()
{
	unlink("/tmp/socktest");
	UnixFDTest s;

	s.Start();
	s.WaitJoined();

	UnixStreamClientSocket c("/tmp/socktest");

	int fd = c.ReceiveFd();
	char buf[1024];
	int rd = read(fd,buf,sizeof(buf));
	CPPUNIT_ASSERT( rd > 0 );
	buf[rd]=0;
	CPPUNIT_ASSERT_EQUAL(0, strcmp(buf,"Hello World!"));
	close(fd);
}
