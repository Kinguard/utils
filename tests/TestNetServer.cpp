#include "TestNetServer.h"

CPPUNIT_TEST_SUITE_REGISTRATION ( TestNetServer );
#include <unistd.h>

#include "NetServer.h"

#include "Thread.h"
#include "Socket.h"
#include "Condition.h"
#include "Logger.h"

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
		Utils::log << Utils::Logger::Debug << "Read ("<<r<<") ["<<recbuf<<"]"<<Utils::end;
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
	NSTester nst("/tmp/nstest");

	nst.Start();
	nst.WaitJoined();
	usleep(500);

	UnixStreamClientSocket client("/tmp/nstest");

	std::string teststring("Hello World!");
	log << Logger::Debug << "Writing server!"<<end;
	client.Write(teststring.c_str(), teststring.length());
	char buf[80];
	ssize_t read = client.Read(buf,sizeof(buf));

	CPPUNIT_ASSERT(read > 0);
	std::string resp(buf);
	CPPUNIT_ASSERT_EQUAL(teststring, resp);

}
