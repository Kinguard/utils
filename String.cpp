/*
 * String.cpp
 *
 *  Created on: 21 maj 2012
 *      Author: tor
 */


#include "String.h"
#include "FileUtils.h"

#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace Utils{

namespace String{

string Trimmed( const string& str, const char* sepSet )
{
	string::size_type const first = str.find_first_not_of(sepSet);
	return ( first == string::npos ) ?
		string():
		str.substr( first, str.find_last_not_of( sepSet ) - first + 1 );
}

string ToLower( string str )
{
	int (*pf)(int)=tolower;
	transform(str.begin(), str.end(), str.begin(), pf);
	return str;
}

string ToUpper( string str )
{
	int (*pf)(int)=toupper;
	transform(str.begin(), str.end(), str.begin(), pf);
	return str;
}

string Chomp( const string& str)
{
	string::size_type pos = str.find_last_not_of("\n");
	return pos==string::npos?str:str.substr(0, pos+1);
}

list<string> Split(const string& str, const char* delim, ssize_t limit)
{
	list<string> items;
	int pos=-1,oldpos=0;
	do {
		pos=str.find(delim,oldpos);
		if ( oldpos>=0 ) {
			string item;
			// Found match
			if( limit == -1 || limit > 1 ){
				item = Trimmed(str.substr(oldpos,pos-oldpos),delim);
			}else if( limit == 1){
				item = Trimmed(str.substr(oldpos,str.size()-oldpos),delim);
			}

			item = Trimmed(item," ");
			if( item.length()>0){
				items.push_back(item);
				limit = limit == -1? -1: limit-1;
			}
		}
		oldpos=pos+1;
    }while ( (limit == -1 || limit > 0) && oldpos>0 );

	return items;
}

string UUID(void)
{
	if( File::FileExists("/proc/sys/kernel/random/uuid") ){
		return File::GetContentAsString("/proc/sys/kernel/random/uuid");
	}
	//TODO: implement a fallback to create a UUID
	throw std::runtime_error("Unable to construct uuid. Please implement fallback");
}

} // Namespace String

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


}// Namespace Utils

