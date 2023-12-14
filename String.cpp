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

#include "String.h"
#include "FileUtils.h"

#include <cstring>
#include <cerrno>

#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <array>
#include <cmath>

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
	string::size_type pos = str.find_last_not_of('\n');
	return pos==string::npos?str:str.substr(0, pos+1);
}


template<class OutputIterator>
static void split(const string& str, OutputIterator it,  const char* delim, ssize_t limit)
{
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
				(*it) = item;
				it++;
				limit = limit == -1? -1: limit-1;
			}
		}
		oldpos=pos+1;
    }while ( (limit == -1 || limit > 0) && oldpos>0 );

}

list<string> Split(const string& str, const char* delim, ssize_t limit)
{
	list<string> items;

	split(str, back_inserter(items), delim, limit);

	return items;
}

void Split(const string& str, vector<string>& vals, const char* delim,
		ssize_t limit) {
	split(str, back_inserter(vals), delim, limit);
}

void Split(const string& str, list<string>& vals, const char* delim,
		ssize_t limit) {
	split(str, back_inserter(vals), delim, limit);
}

string UUID()
{
	if( File::FileExists("/proc/sys/kernel/random/uuid") ){
		return File::GetContentAsString("/proc/sys/kernel/random/uuid");
	}
	//TODO: implement a fallback to create a UUID
	throw std::runtime_error("Unable to construct uuid. Please implement fallback");
}

string Join(const list<string> &elems, const string &separator)
{
	stringstream ss;
	bool first = true;
	for( const auto& elem: elems)
	{
		if( first )
		{
			ss << elem;
			first = false;
		}
		else
		{
			ss << separator;
			ss << elem;
		}
	}
	return ss.str();
}


string ToHuman(uint64_t value, const string &suffix, uint base)
{
	constexpr uint8_t DECIMAL = 10;
	static const array<string, 8> units = {" ", " K", " M", " G", " T", " P", " E"};
	int i = 0;

	long double val = value;
	long double integral = 0;
	long double frac = 0;

	while( val >= base )
	{
		val /= base;
		i++;
	}

	frac = modfl(val, &integral);
	uint fractasint = round(frac*DECIMAL);
	std::stringstream ret;

	if( i == 0  || fractasint == 0)
	{
		ret << integral << units.at(i) << suffix;
		//ret << fixed << setprecision(0) << val << units.at(i);
	}
	else
	{
		ret << fixed << setprecision(1) << val << units.at(i) << suffix;
	}

	return ret.str();
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

constexpr uint NUMCHARS = 0x10;
void HexDump(const void* data, uint len){
	const unsigned char* buf = static_cast<const unsigned char*>( data );

	std::cout << "---------------- Hexdump "<< std::dec <<len << " ("<< std::hex << len<<") bytes ----------------"<<std::endl;

	for( uint i = 0; i < len; i+= NUMCHARS){
		std::cout << std::setw(6) << std::setfill('0') << std::hex << i << ": ";

		for( uint j = 0; j < NUMCHARS; j++){
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

