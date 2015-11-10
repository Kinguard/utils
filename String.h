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

#ifndef STRING_H_
#define STRING_H_

#include <string>
#include <list>
#include <vector>

#include <sys/types.h>

using namespace std;

namespace Utils{

namespace String {
	string Trimmed( const string& str, const char* sepSet );
	string ToLower( string str );
	string ToUpper( string str );
	string Chomp( const string& str);
	list<string> Split(const string& str, const char* delim=" ", ssize_t limit=-1);
	void Split(const string& str, vector<string>& vals, const char* delim=" ", ssize_t limit=-1);
	void Split(const string& str, list<string>& vals, const char* delim=" ", ssize_t limit=-1);
	string UUID(void);
}

std::string Errno(const std::string& str="");

void HexDump(const void* data, int len);

}

#endif /* STRING_H_ */
