/*
 * String.h
 *
 *  Created on: 21 maj 2012
 *      Author: tor
 */

#ifndef STRING_H_
#define STRING_H_

#include <string>

namespace Utils{

std::string Errno(const std::string& str="");

void HexDump(const void* data, int len);

}

#endif /* STRING_H_ */
