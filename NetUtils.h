/*
 * NetUtils.h
 *
 *  Created on: Jul 8, 2012
 *      Author: tor
 */

#ifndef NETUTILS_H_
#define NETUTILS_H_

#include <sys/socket.h>
#include <sys/types.h>

#include <string>
#include <vector>
#include <list>

#include "Exceptions.h"

namespace Utils{
namespace Net{

	struct sockaddr GetIfAddr(const std::string& ifname);
	std::vector<uint8_t> GetHWAddr(const std::string& ifname);
	std::list<std::string> GetInterfaces(void);
}//NS Net

} //NS Utils

#endif /* NETUTILS_H_ */
