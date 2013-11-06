/*
 * Socket.h
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "Exceptions.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/un.h>
#include <netdb.h>

namespace Utils{

namespace Net {

class Socket{
protected:
	int domain;
	int type;
	int sock;
	std::string hostpath;
	std::string service;
public:
	enum Domain{
		NoDomain = -1,
		Local = AF_LOCAL,
		IPV4 = AF_INET,
		IPV6 = AF_INET6
	};

	enum Type{
		NoType = -1,
		Stream = SOCK_STREAM,
		Datagram = SOCK_DGRAM
	};

	Socket(enum Socket::Domain domain, enum Socket::Type type);
	Socket(const std::string& hostpath, const std::string& service, enum Socket::Domain domain, enum Socket::Type type);
	Socket(int sockfd);

	int getSocketFd(){ return this->sock; }

	virtual size_t Write(const void* buf, size_t len);
	virtual size_t Write(const std::vector<char>& v);

	virtual size_t Read(void* buf, size_t len);

	//Read at most vector::capacity bytes and store into vector
	virtual size_t Read(std::vector<char>& v);
	//Read at most len bytes and append to v
	virtual size_t AppendTo(std::vector<char>& v, size_t len);

	//TODO: Implement a close method and check for a closed connection in other methods.
	virtual ~Socket();

};
typedef std::shared_ptr<Socket> SocketPtr;

class ClientSocket: public Socket{
public:
	ClientSocket(enum Socket::Domain domain, enum Socket::Type type);
	ClientSocket(const std::string& hostpath, const std::string& service,
			enum Socket::Domain domain, enum Socket::Type type);
	virtual ~ClientSocket(){};
protected:
	void Connect(void);
	bool connected;
};
typedef std::shared_ptr<ClientSocket> ClientSocketPtr;

class ServerSocket: public Socket{
private:
	struct timeval timeout;
	bool timedout;
	bool bound;
protected:
	void Bind(void);
public:
	ServerSocket(enum Socket::Domain domain, enum Socket::Type type);
	ServerSocket(const std::string& hostpath, const std::string& service,
			enum Socket::Domain domain, enum Socket::Type type);
	virtual Utils::Net::SocketPtr Accept(void);
	void SetTimeout(long sec, long usec);
	bool TimedOut(void);
	virtual ~ServerSocket(){};
};
typedef std::shared_ptr<ServerSocket> ServerSocketPtr;

class TCPClientSocket: public ClientSocket{

public:
	TCPClientSocket(const std::string& host,uint16_t port);

	virtual ~TCPClientSocket(){};
};
typedef std::shared_ptr<TCPClientSocket> TCPClientSocketPtr;

class TCPServerSocket: public ServerSocket{
public:
	/*
	 * Interface, interface or "" for alla (INADDR_ANY)
	 */
	TCPServerSocket(const std::string& interface,uint16_t port);

	virtual ~TCPServerSocket(){};
};
typedef std::shared_ptr<TCPServerSocket> TCPServerSocketPtr;

class UDPClientSocket: public ClientSocket{
public:
	/*
	 * Only
	 */
	UDPClientSocket(void):ClientSocket(Socket::IPV4,Socket::Datagram){};
	UDPClientSocket(const std::string& host, uint16_t port);

	size_t SendTo(const std::string& host, uint16_t port, const char* buff, size_t len);
	size_t SendTo(const std::string& host, uint16_t port, const std::vector<char>& data);
	//TODO: Maybe implement recvfrom?
};
typedef std::shared_ptr<UDPClientSocket> UDPClientSocketPtr;

class UDPServerSocket: public ServerSocket{
public:
	/*
	 * Hostpath, interface to bind to or empty
	 */
	UDPServerSocket(const std::string& interface, const std::string& service);
	virtual Utils::Net::SocketPtr Accept(void);
	virtual ~UDPServerSocket(){};
};
typedef std::shared_ptr<UDPServerSocket> UDPServerSocketPtr;

class MulticastSocket: public UDPServerSocket {
public:

	/*
	 * service - Which port to listen on
	 */
	MulticastSocket(const std::string& service);

	/*
	 * interface - which IP to listen on
	 * service - which port to use
	 */
	MulticastSocket(const std::string& interface, const std::string& service);

	/*
	 *  ip - Remote ip to join
	 *  iface - Local interface to listen on
	 */
	void Join(const std::string& ip, const std::string& iface);
	void Leave(const std::string& ip, const std::string& iface);

	void SetTTL(int ttl);
	int GetTTL(void);

	void SetLoopback(bool loop);
	bool GetLoopback(void);

};
typedef std::shared_ptr<MulticastSocket> MulticastSocketPtr;

class UnixStreamClientSocket: public ClientSocket {
public:
	UnixStreamClientSocket(const std::string& path);
	void SendFd(int fd);
	int ReceiveFd();
	struct ucred GetCredentials(void);
};
typedef std::shared_ptr<UnixStreamClientSocket> UnixStreamClientSocketPtr;


class UnixStreamServerSocket: public ServerSocket {
public:
	UnixStreamServerSocket(const std::string& path);

	virtual ~UnixStreamServerSocket();
};
typedef std::shared_ptr<UnixStreamServerSocket> UnixStreamServerSocketPtr;

}
}
#endif /* SOCKET_H_ */
