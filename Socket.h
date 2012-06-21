/*
 * Socket.h
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>
#include <cstdint>

#include "Exceptions.h"
#include <sys/socket.h>
#include <sys/types.h>

namespace Utils{

class Socket{
protected:
	int domain;
	int type;
	int sock;

public:
	Socket(void);
	Socket(int domain, int type);
	Socket(int sockfd);

	int getSocketFd(){ return this->sock; }

	virtual size_t Write(const void* buf, size_t len);
	virtual size_t Read(void* buf, size_t len);

	virtual ~Socket();

	enum{
		Local = AF_LOCAL,
		IPV4 = AF_INET,
		IPV6 = AF_INET6
	};

	enum{
		Stream = SOCK_STREAM,
		Datagram = SOCK_DGRAM
	};
};

class InetSocket: public Socket{
protected:
	InetSocket(): Socket(){};
	InetSocket(int domain, int type, const std::string& host, uint16_t port):
		Socket(domain, type),host(host), port(port)	{};
	void connect(void);

private:
	std::string host;
	uint16_t port;
};

class TCPClient: public InetSocket{

public:
	TCPClient(void);
	TCPClient(const std::string& host,uint16_t port);

};

class TCPServer: public Socket{

};

class UDPSocket: public InetSocket{
public:
	UDPSocket(void);
	UDPSocket(const std::string& host,uint16_t port);
};

}



#endif /* SOCKET_H_ */
