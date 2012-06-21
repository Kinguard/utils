/*
 * String.h
 *
 *  Created on: 21 maj 2012
 *      Author: tor
 */

#ifndef STRING_H_
#define STRING_H_

#include <string>
#include <list>

using namespace std;

namespace Utils{

namespace String {
	string Trimmed( const string& str, const char* sepSet );
	string ToLower( string str );
	string ToUpper( string str );
	string Chomp( const string& str);
	list<string> Split(const string& str, const char delim);
}

std::string Errno(const std::string& str="");

void HexDump(const void* data, int len);

}

#endif /* STRING_H_ */
