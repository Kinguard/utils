/*

	libutils - a collection of usable tools

	Copyright (c) 2015 Tor Krill <tor@openproducts.com>

	This file is part of libutils

	libutils is  free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "NetUtils.h"

struct sockaddr Utils::Net::GetIfAddr(const std::string& ifname) {
	struct ifreq req {};

	memset( &req, 0, sizeof( struct ifreq ) );
	sprintf( req.ifr_name, "%s", ifname.c_str() );

	int sock = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sock < 0){
		throw Utils::ErrnoException("Unable to create socket");
	}

	if( ioctl( sock, SIOCGIFADDR, &req ) < 0 ){
		throw ErrnoException("Unable to get address");
	}

	close( sock );

	return req.ifr_addr;
}

sockaddr Utils::Net::GetNetmask(const std::string &ifname)
{
	struct ifreq req{};

	memset( &req, 0, sizeof(struct ifreq) );
	sprintf( req.ifr_name, "%s",ifname.c_str() );

	int sock = socket( AF_INET,SOCK_DGRAM, 0 );

	if( sock < 0){
		throw Utils::ErrnoException("Unable to create socket");
	}

	if( ioctl( sock, SIOCGIFNETMASK, &req ) < 0 )
	{
		throw ErrnoException("Unable to get netmask from interface");
	}

	return req.ifr_netmask;
}

std::vector<uint8_t> Utils::Net::GetHWAddr(const std::string& ifname) {
	struct ifreq interface;

	strncpy(interface.ifr_ifrn.ifrn_name,ifname.c_str(),IFNAMSIZ-1);
	interface.ifr_ifrn.ifrn_name[IFNAMSIZ-1]=0;

	int sock = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sock < 0){
		throw Utils::ErrnoException("Unable to create socket");
	}

	if(ioctl(sock,SIOCGIFHWADDR,&interface)<0){
		throw ErrnoException("Unable to get hw-address from socket");
	}

	return std::vector<uint8_t>(interface.ifr_hwaddr.sa_data,interface.ifr_hwaddr.sa_data+IFHWADDRLEN);
}

std::list<std::string> Utils::Net::GetInterfaces(void) {
	std::list<std::string> res;

	int sock = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sock < 0){
		throw Utils::ErrnoException("Unable to create socket");
	}

	char buf[1024];
	struct ifconf ifc;

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;

	if(ioctl(sock, SIOCGIFCONF, &ifc) < 0){
		throw Utils::ErrnoException("Unable to query interfaces");
	}

	int nIfs = ifc.ifc_len / sizeof(struct ifreq);
	struct ifreq *ifr = ifc.ifc_req;
	for(int i = 0; i < nIfs; i++){
		struct ifreq *dev = &ifr[i];
		res.push_back(dev->ifr_name);
	}
	return res;
}

std::string Utils::Net::SockAddrToString(sockaddr *address)
{
	std::string ret;
	switch( address->sa_family ){
	case AF_UNIX :
		throw std::runtime_error("Currently unsupported network class AF_UNIX");
		break;
	case AF_INET:
	{
		struct sockaddr_in *addr = ( struct sockaddr_in* ) address;
		return inet_ntoa( addr->sin_addr );
		break;
	}
	case AF_INET6:
		throw std::runtime_error("Currently unsupported network class AF_INET6");
		break;
	case AF_UNSPEC:
		throw std::runtime_error("Currently unsupported network class AF_UNSPEC");
		break;
	default:
		throw std::runtime_error("Unknown network class");
	}
	return ret;
}
