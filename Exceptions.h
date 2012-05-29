/*
 * Exceptions.h
 *
 *  Created on: 21 maj 2012
 *      Author: tor
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <string>
#include <cstring>
#include <cerrno>

namespace Utils{

class ErrnoException: public std::runtime_error{
public:
	explicit ErrnoException(const std::string& what):runtime_error(what+" ("+std::string(strerror(errno))+")"){
	}
};


}
#endif /* EXCEPTIONS_H_ */
