/*
 * Socket.cpp
 *
 *  Created on: 19 jun 2012
 *      Author: tor
 */

#include "Socket.h"

#include <sstream>

#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

/*
 * Base class implementation for socket
 */

Utils::Socket::Socket(void): type(-1), sock(-1)
{
}

Utils::Socket::Socket(int sockfd): type(-1),sock(sockfd)
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

void Utils::InetSocket::connect(void)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = this->domain;
	hints.ai_socktype = this->type;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	std::stringstream ss;

	ss << port;

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
	this->connect();
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
	this->connect();
}
