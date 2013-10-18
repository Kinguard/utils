/*
 * NetServer.h
 *
 *  Created on: Oct 18, 2013
 *      Author: tor
 */

#ifndef NETSERVER_H_
#define NETSERVER_H_

#include "ClassTools.h"
#include "Socket.h"
#include "Mutex.h"

namespace Utils
{
namespace Net
{

class NetServer: public NoCopy
{
public:
	NetServer (ServerSocketPtr serv, int timeout);

	virtual void Dispatch(SocketPtr con);
	void Run();
	void ShutDown();

	virtual	~NetServer ();
protected:
	void decreq();

private:
	bool shutdown;
	int pendingreq;
	Mutex m;
	ServerSocketPtr serv;
	void increq();
};

} /* namespace Net */
} /* namespace Utils */
#endif /* NETSERVER_H_ */
