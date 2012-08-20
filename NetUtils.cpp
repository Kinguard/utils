/*
 * NetUtils.cpp
 *
 *  Created on: Jul 8, 2012
 *      Author: tor
 */

#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "NetUtils.h"

struct sockaddr Utils::Net::GetIfAddr(const std::string& ifname) {
	int ret;
	struct ifreq req;

	bzero( &req, sizeof( struct ifreq ) );
	sprintf( req.ifr_name, "%s", ifname.c_str() );

	int sock = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sock < 0){
		throw Utils::ErrnoException("Unable to create socket");
	}

	if( ( ret = ioctl( sock, SIOCGIFADDR, &req ) ) < 0 ){
		throw ErrnoException("Unable to get address");
	}

	close( sock );

	return req.ifr_addr;
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
