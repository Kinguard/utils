/*
 * Socket.cpp
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#include "Socket.h"
#include "NetUtils.h"

#include <sstream>

#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

/*
 * Base class implementation for socket
 */

Utils::Socket::Socket(void): domain(-1),type(-1), sock(-1)
{
}

Utils::Socket::Socket(int sockfd): domain(-1), type(-1),sock(sockfd)
{
}

Utils::Socket::Socket(int domain, int type):domain(domain), type(type)
{
	this->sock = socket(this->domain, this->type, 0);
	if( this->sock < 0 ){
		throw Utils::ErrnoException("Failed to create socket");
	}
}

size_t Utils::Socket::Write(const void* buf, size_t len)
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

size_t Utils::Socket::Write(const std::vector<char>& v) {
	return this->Write(&v[0], v.size());
}


size_t Utils::Socket::Read(void* buf, size_t len)
{
	ssize_t ret;

	if ( ( ret = recv( this->sock, buf, len, 0) ) < 0 ) {
		throw Utils::ErrnoException("Unable to receive data");
	}

	return ret;
}

size_t Utils::Socket::Read(std::vector<char>& v)
{
	v.resize(v.capacity());
	size_t size = this->Read(&v[0], v.capacity());
	v.resize(size);
	return size;
}

size_t Utils::Socket::AppendTo(std::vector<char>& v, size_t len) {
	size_t cur_size = v.size();
	v.resize(cur_size+len);

	size_t rd = this->Read(&v[cur_size],len);
	v.resize(cur_size+rd);

	return rd;
}

Utils::Socket::~Socket()
{
	if( this->sock >= 0 ){
		close(this->sock);
	}
}


/*
 * InetSocket implementation
 */

void Utils::InetSocket::Connect(const std::string& host, uint16_t port)
{
	if( this->connected && this->type == Socket::Stream ){
		throw std::runtime_error("Already connected");
	}

	if( host != "" ){
		this->host=host;
	}

	if( port != 0 ){
		this->port = port;
	}

	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = this->domain;
	hints.ai_socktype = this->type;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	std::stringstream ss;

	ss << this->port;

	int res = getaddrinfo(this->host.c_str(), ss.str().c_str(), &hints, &result);
	if( res != 0 ){
		throw Utils::ErrnoException("Unable to resolve address");
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if( ::connect(this->sock, rp->ai_addr, rp->ai_addrlen) != -1){
			break; // Success
		}
	}

	if( rp == NULL ){
		throw std::runtime_error("Failed to connect");
	}

	freeaddrinfo(result);
	this->connected = true;
}

void Utils::InetSocket::Bind(uint16_t port, const std::string& interface)
{
	if( this->bound ){
		throw std::runtime_error("Socket already bound");
	}

	struct sockaddr_in addr;

	bzero( &addr, sizeof( struct sockaddr_in ) );
	addr.sin_family=AF_INET;

	if( interface != "" ){
		struct sockaddr ifaddr =  Utils::Net::GetIfAddr(interface) ;
		addr.sin_addr.s_addr = ((struct sockaddr_in*) &ifaddr)->sin_addr.s_addr;
	}else{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	addr.sin_port=htons(port);

	int optval = 1;
	if( setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
		throw ErrnoException("Failed to set reuse on socket");
	}

	if ( ::bind(this->sock, (struct sockaddr *) &addr, sizeof( addr ) ) < 0) {
		throw ErrnoException("Failed to bind socket");
	}
	this->bound = true;
}


/*
 * TCP Client implementation
 */

Utils::TCPClient::TCPClient(void): InetSocket(Socket::IPV4, Socket::Stream,"",0)
{
}

Utils::TCPClient::TCPClient(const std::string& host, uint16_t port):
		InetSocket(Socket::IPV4, Socket::Stream, host, port)
{
	this->Connect();
}

/*
 * UDP socket implementation
 */

Utils::UDPSocket::UDPSocket(void): InetSocket(Socket::IPV4, Socket::Datagram, "",0)
{
}

Utils::UDPSocket::UDPSocket(const std::string& host, uint16_t port):
		InetSocket(Socket::IPV4, Socket::Datagram, host, port)
{
	this->Connect();
}

size_t Utils::UDPSocket::SendTo(const std::string& host, uint16_t port,
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

size_t Utils::UDPSocket::SendTo(const std::string& host, uint16_t port,
		const std::vector<char>& data)
{
	return this->SendTo(host, port, &data[0], data.size() );
}

/*
 * Multicast socket implementation
 */


void Utils::MulticastSocket::Join(const std::string& ip, const std::string& iface)
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

void Utils::MulticastSocket::Leave(const std::string& ip, const std::string& iface)
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

void Utils::MulticastSocket::SetTTL(int ttl)
{
	if (setsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl)) < 0) {
		throw ErrnoException("Failed to set multicast ttl");
	}
}

int Utils::MulticastSocket::GetTTL(void)
{
	int ttl;
	socklen_t optlen = sizeof(ttl);

	if (getsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,&optlen) < 0) {
		throw ErrnoException("Failed to get multicast ttl");
	}

	return ttl;
}

void Utils::MulticastSocket::SetLoopback(bool loop) {
	int arg = loop;

	if (setsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_LOOP,&arg,sizeof(arg)) < 0) {
		throw ErrnoException("Failed to set multicast loopback mode");
	}
}

bool Utils::MulticastSocket::GetLoopback(void) {
	int loop;
	socklen_t optlen = sizeof(loop);

	if (getsockopt(this->sock,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,&optlen) < 0) {
		throw ErrnoException("Failed to get multicast loopback mode");
	}

	return loop;
}





