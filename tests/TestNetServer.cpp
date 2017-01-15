#include "TestNetServer.h"

CPPUNIT_TEST_SUITE_REGISTRATION ( TestNetServer );
#include <unistd.h>

#include "NetServer.h"

#include "Thread.h"
#include "Socket.h"
#include "Condition.h"
#include "Logger.h"
#include "FileUtils.h"

using namespace Utils;
using namespace Utils::Net;

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

class EchoServer: public Utils::Net::NetServer
{
public:
	EchoServer(ServerSocketPtr serv, int timeout): NetServer(serv,timeout) {}

	virtual void Dispatch( SocketPtr con){
		char recbuf[1024];
		ssize_t r=con->Read(recbuf, 1024);
		Utils::logg << Utils::Logger::Debug << "Read ("<<r<<") ["<<recbuf<<"]"<<Utils::lend;
		if(r>0){
			con->Write(recbuf, r);
		}
		this->decreq();
	}

	virtual ~EchoServer(){}
};

class NSTester: public TestServer
{
private:
	EchoServer e;
public:

	NSTester(const std::string& path):
		e(UnixStreamServerSocketPtr(new UnixStreamServerSocket(path)),5),
		TestServer(true)
		{}

	virtual void Run()
	{
		this->joined.Notify();
		e.Run();
	}
};

void TestNetServer::setUp()
{
}

void TestNetServer::tearDown()
{
}

void TestNetServer::Test()
{
	//Utils::log.SetLevel(Utils::Logger::Debug);

	if( Utils::File::SocketExists( "/tmp/nstest" ) )
	{
		Utils::File::Delete( "/tmp/nstest");
	}

	NSTester nst("/tmp/nstest");

	nst.Start();
	nst.WaitJoined();
	usleep(500);

	UnixStreamClientSocket client("/tmp/nstest");

	std::string teststring("Hello World!");
	logg << Logger::Debug << "Writing server!"<<lend;
	client.Write(teststring.c_str(), teststring.length());
	char buf[81];
	ssize_t read = client.Read(buf,sizeof(buf)-1);
	buf[read] = 0;

	CPPUNIT_ASSERT(read > 0);
	CPPUNIT_ASSERT_EQUAL( (size_t)read, teststring.length() );
	std::string resp(buf);
	CPPUNIT_ASSERT_EQUAL(teststring, resp);

}
