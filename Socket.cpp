/*
 * Socket.cpp
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#include "Socket.h"
#include "NetUtils.h"
#include "FileUtils.h"
#include <sstream>
#include <string>

#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

using namespace Utils::Net;

/*
 * Base class implementation for socket
 */

Socket::Socket(int sockfd): domain(Socket::NoDomain),type(Socket::NoType)
{
	this->sock = sockfd;
}

Socket::Socket(enum Socket::Domain domain, enum Socket::Type type):
		Socket("","",domain,type)
{
}

Socket::Socket ( const std::string& hostpath, const std::string& service,
		enum Socket::Domain domain, enum Socket::Type type ):
		domain(domain), type(type), hostpath(hostpath),service(service)
{
	this->sock = socket(this->domain, this->type, 0);
	if( this->sock < 0 ){
		throw Utils::ErrnoException("Failed to create socket");
	}
}

size_t Socket::Write(const void* buf, size_t len)
{
	ssize_t ret;
	size_t wt=0;
	do{
		ret = send( this->sock, (char*)buf+wt, len-wt, 0);
		if ( ret < 0 ) {
			throw Utils::ErrnoException("Unable to send data");
		}
		wt += ret;
	}while( ret >=0  && wt < len);

	return ret;
}

size_t Socket::Write(const std::vector<char>& v) {
	return this->Write(&v[0], v.size());
}


size_t Socket::Read(void* buf, size_t len)
{
	ssize_t ret;

	if ( ( ret = recv( this->sock, buf, len, 0) ) < 0 ) {
		throw Utils::ErrnoException("Unable to receive data");
	}

	return ret;
}

size_t Socket::Read(std::vector<char>& v)
{
	v.resize(v.capacity());
	size_t size = this->Read(&v[0], v.capacity());
	v.resize(size);
	return size;
}

size_t Socket::AppendTo(std::vector<char>& v, size_t len) {
	size_t cur_size = v.size();
	v.resize(cur_size+len);

	size_t rd = this->Read(&v[cur_size],len);
	v.resize(cur_size+rd);

	return rd;
}

Socket::~Socket()
{
	if( this->sock >= 0 ){
		close(this->sock);
	}
}

/*
 * Client socket implementation
 */

ClientSocket::ClientSocket ( enum Socket::Domain domain,
		enum Socket::Type type ):
				Socket(domain, type),connected(false)
{
}

ClientSocket::ClientSocket ( const std::string& hostpath,
		const std::string& service, enum Socket::Domain domain,
		enum Socket::Type type ):
				Socket(hostpath, service, domain, type),connected(false)
{
}

void
ClientSocket::Connect ( void )
{
	if( this->connected && this->type == Socket::Stream ){
		throw std::runtime_error("Already connected");
	}
	/*
	 * This got a bit stupid wrg on inet vs unix sockets then
	 * consider splitting up inheritance?
	 */
	switch( this->domain )
	{
	case Socket::IPV4:
	case Socket::IPV6:
	{
		struct addrinfo addr_hints;
		struct addrinfo *addr_result;

		memset(&addr_hints, 0, sizeof(struct addrinfo));
		addr_hints.ai_family = this->domain;
		addr_hints.ai_socktype = this->type;
		addr_hints.ai_flags = 0;
		addr_hints.ai_protocol = 0;

		int res = getaddrinfo(this->hostpath.c_str(), this->service.c_str(), &addr_hints, &addr_result);
		if( res != 0 ){
			throw Utils::ErrnoException("Unable to resolve address");
		}

		struct addrinfo *rp;
		for (rp = addr_result; rp != NULL; rp = rp->ai_next) {
			if( ::connect(this->sock, rp->ai_addr, rp->ai_addrlen) != -1){
				break; // Success
			}
		}

		freeaddrinfo(addr_result);

		if( rp == NULL ){
			throw std::runtime_error("Failed to connect");
		}
	}
		break;
	case Socket::Local:
	{
		struct sockaddr_un addr;

		memset(&addr, 0, sizeof(struct sockaddr_un));
		addr.sun_family = this->domain;
		strncpy(addr.sun_path, this->hostpath.c_str(), this->hostpath.length() );

		if( ::connect(this->sock, (const sockaddr*) &addr, sizeof(struct sockaddr_un)) == -1){
			throw std::runtime_error("Failed to connect");
		}

	}
		break;
	default:
		throw std::runtime_error("Unsupported socket domain in connect");
	}


	this->connected = true;

}

/*
 * Server socket implementation
 */

ServerSocket::ServerSocket ( enum Socket::Domain domain,
		enum Socket::Type type ):
				Socket(domain, type), timeout({0,0}),timedout(false), bound(false)
{
}

ServerSocket::ServerSocket ( const std::string& hostpath,
		const std::string& service, enum Socket::Domain domain,
		enum Socket::Type type ):
				Socket(hostpath, service, domain, type),  timedout(false), bound(false)
{
}

#include <iostream>
void
ServerSocket::Bind ( void )
{
	if( this->bound ){
		throw std::runtime_error("Socket already bound");
	}

	std::shared_ptr<struct sockaddr> addr;
	socklen_t len;

	switch( this->domain )
	{
	case Socket::IPV4:
	{
		struct sockaddr_in *addr_in = new struct sockaddr_in;
		addr.reset( (struct sockaddr*) addr_in);
		len = sizeof(struct sockaddr_in);

		bzero( addr_in, sizeof( struct sockaddr_in ) );

		if( this->hostpath != "" ){
			struct sockaddr ifaddr =  Utils::Net::GetIfAddr( this->hostpath );
			addr_in->sin_addr.s_addr = ((struct sockaddr_in*) &ifaddr)->sin_addr.s_addr;
		}else{
			addr_in->sin_addr.s_addr = htonl(INADDR_ANY);
		}
		if(this->service != "")
		{
			uint16_t port = atoi( this->service.c_str() );
			addr_in->sin_port=htons( port );
		}
	}
		break;
	case Socket::Local:
	{
		struct sockaddr_un *addr_un = new struct sockaddr_un;
		addr.reset( (struct sockaddr*) addr_un);
		len = sizeof(struct sockaddr_un);

		bzero( addr_un, sizeof( struct sockaddr_un ) );

		strncpy(addr_un->sun_path, this->hostpath.c_str(), this->hostpath.length());

	}
		break;
	default:
		throw std::runtime_error("Unsupported socket domain in bind");
	}

	int optval = 1;
	if( setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
		throw ErrnoException("Failed to set reuse on socket");
	}


	addr->sa_family= this->domain;

	if ( ::bind(this->sock, addr.get(), len ) < 0) {
		throw ErrnoException("Failed to bind socket");
	}
	this->bound = true;
}

SocketPtr
ServerSocket::Accept ()
{
	if(listen(this->sock,5)<0){
		throw new std::runtime_error("Failed to listen for connection");
	}
	struct timeval* timer=NULL;
	struct timeval timercp;
	if(timeout.tv_sec>0 || timeout.tv_usec>0){
		// Use a copy of our timeval since it will be updated by system
		timercp=this->timeout;
		timer=&timercp;
	}

	fd_set r_set;
	int selret;
	FD_ZERO(&r_set);
	FD_SET(this->sock,&r_set);
	if((selret=select(this->sock+1,&r_set,NULL,NULL,timer)>0)){
		int clientsock;
		struct sockaddr_un addr;
		socklen_t addr_len=sizeof(sockaddr_un);
		if((clientsock=accept(this->sock,(struct sockaddr *)&addr, &addr_len))<0){
			throw ErrnoException("Failed to accept new connection");
		}
		return SocketPtr( new Socket(clientsock) );
	}else{
		this->timedout=selret==0;
		return SocketPtr(NULL);
	}
}

void
ServerSocket::SetTimeout ( long sec, long usec )
{
	this->timeout.tv_sec=sec;
	this->timeout.tv_usec=usec;
}

bool
ServerSocket::TimedOut ()
{
	return this->timedout;
}

/*
 * TCP Client implementation
 */

TCPClientSocket::TCPClientSocket(const std::string& host, uint16_t port):
		ClientSocket(Socket::IPV4, Socket::Stream)
{
	this->hostpath = host;

	std::stringstream ss;
	ss << port;

	this->service = ss.str();

	this->Connect();
}

/*
 * TCP Server implementation
 */
TCPServerSocket::TCPServerSocket ( const std::string& interface, uint16_t port ):
		ServerSocket(Socket::IPV4, Socket::Stream)
{
	this->hostpath = interface;

	std::stringstream ss;
	ss << port;

	this->service = ss.str();

	this->Bind();
}


/*
 * UDP  client socket implementation
 */

UDPClientSocket::UDPClientSocket(const std::string& host, uint16_t port):
		ClientSocket(Socket::IPV4, Socket::Datagram)
{
	this->hostpath = host;

	std::stringstream ss;
	ss << port;

	this->service = ss.str();

	this->Connect();
}

size_t UDPClientSocket::SendTo(const std::string& host, uint16_t port,
		const char* buff, size_t len)
{
	struct addrinfo hints;
	struct addrinfo *result;
	size_t written=0;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = this->domain;
	hints.ai_socktype = this->type;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	std::stringstream ss;

	ss << port;

	int res = getaddrinfo(host.c_str(), ss.str().c_str(), &hints, &result);
	if( res != 0 ){
		throw Utils::ErrnoException("Unable to resolve address");
	}

	if( result != NULL ){
		written = sendto(this->sock, buff, len, 0,result->ai_addr, result->ai_addrlen);
	}

	freeaddrinfo(result);

	return written;
}

size_t UDPClientSocket::SendTo(const std::string& host, uint16_t port,
		const std::vector<char>& data)
{
	return this->SendTo(host, port, &data[0], data.size() );
}

/*
 * UDP server socket implementation
 */
UDPServerSocket::UDPServerSocket(const std::string& hostpath, const std::string& service):
		ServerSocket(hostpath, service, Socket::IPV4, Socket::Datagram)
{
	this->Bind();
}

SocketPtr
UDPServerSocket::Accept ( void )
{
	throw std::runtime_error("Error trying to accept on non stream socket");
}


/*
 * Multicast socket implementation
 */

MulticastSocket::MulticastSocket ( const std::string& service ):
		UDPServerSocket("", service)
{
}


MulticastSocket::MulticastSocket ( const std::string& interface, const std::string& service ):
		UDPServerSocket(interface, service)
{
}

void MulticastSocket::Join(const std::string& ip, const std::string& iface)
{
	struct ip_mreq mreq;

	mreq.imr_multiaddr.s_addr = inet_addr(ip.c_str());
	if( iface == "" ){
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	}else{
		//TODO: This might be wrong, possibly use IP_MULTICAST_IF ioctl instead
		struct sockaddr ifaddr =  Utils::Net::GetIfAddr(iface) ;
		mreq.imr_interface.s_addr = ((struct sockaddr_in*) &ifaddr)->sin_addr.s_addr;
	}

	if (setsockopt(this->sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		throw ErrnoException("Failed to join multicast group");
	}

}

void MulticastSocket::Leave(const std::string& ip, const std::string& iface)
{
	struct ip_mreq mreq;

	mreq.imr_multiaddr.s_addr = inet_addr(ip.c_str());
	if( iface == "" ){
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	}else{
		//TODO: This might be wrong, possibly use IP_MULTICAST_IF ioctl instead
		struct sockaddr ifaddr =  Utils::Net::GetIfAddr(iface) ;
		mreq.imr_interface.s_addr = ((struct sockaddr_in*) &ifaddr)->sin_addr.s_addr;
	}

	if (setsockopt(this->sock,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		throw ErrnoException("Failed to join multicast group");
	}
}

void MulticastSocket::SetTTL(int ttl)
{
	if (setsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl)) < 0) {
		throw ErrnoException("Failed to set multicast ttl");
	}
}

int MulticastSocket::GetTTL(void)
{
	int ttl;
	socklen_t optlen = sizeof(ttl);

	if (getsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,&optlen) < 0) {
		throw ErrnoException("Failed to get multicast ttl");
	}

	return ttl;
}

void MulticastSocket::SetLoopback(bool loop) {
	int arg = loop;

	if (setsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_LOOP,&arg,sizeof(arg)) < 0) {
		throw ErrnoException("Failed to set multicast loopback mode");
	}
}

bool MulticastSocket::GetLoopback(void) {
	int loop;
	socklen_t optlen = sizeof(loop);

	if (getsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,&optlen) < 0) {
		throw ErrnoException("Failed to get multicast loopback mode");
	}

	return loop;
}


UnixStreamClientSocket::UnixStreamClientSocket(const std::string& path):
		ClientSocket(path, "", Socket::Local, Socket::Stream)
{
	this->Connect();
}

UnixStreamServerSocket::UnixStreamServerSocket(const std::string& path):
			ServerSocket(path, "", Socket::Local, Socket::Stream)
{
	this->Bind();
}

UnixStreamServerSocket::~UnixStreamServerSocket ()
{
	if( Utils::File::SocketExists( this->hostpath )){
		Utils::File::Delete( this->hostpath );
	}
}
