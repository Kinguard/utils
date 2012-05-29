/*
 * String.cpp
 *
 *  Created on: 21 maj 2012
 *      Author: tor
 */


#include "String.h"
#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <cctype>

namespace Utils{

std::string Errno(const std::string& str){
	std::string ret;
	if(str==""){
		ret = strerror(errno);
	}else{
		ret = str+" ("+strerror(errno)+")";
	}
	return ret;
}

#define NUMCHARS 0x10
void HexDump(const void* data, int len){
	const unsigned char* buf = static_cast<const unsigned char*>( data );

	std::cout << "---------------- Hexdump "<< std::dec <<len << " ("<< std::hex << len<<") bytes ----------------"<<std::endl;

	for( int i = 0; i < len; i+= NUMCHARS){
		std::cout << std::setw(6) << std::setfill('0') << std::hex << i << ": ";

		for( int j = 0; j < NUMCHARS; j++){
			if( i + j < len ){
				std::cout << std::setw(2) << std::setfill('0') << std::hex << (int) buf[i+j]<<" ";
			}else{
				std::cout << "  ";
			}
		}

		std::cout << " ";
		for( int j = 0; j < NUMCHARS; j++){
			if( i + j < len ){
				if( isprint( buf[ i + j ]) ){
					std::cout << buf[ i + j ];
				}else{
					std::cout << ".";
				}
			}
		}
		std::cout << std::endl;
	}

}


}

