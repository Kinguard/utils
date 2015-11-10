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

#include <stdexcept>
#include <algorithm>

#include "Regex.h"

namespace Utils {

Regex::Regex() : valid(false)
{
}

Regex::Regex(const string &pattern, bool insensitive): Regex()
{
	this->Compile( pattern, insensitive );
}

vector<Regex::Match> Regex::DoMatch(const string &line, int maxmatch)
{

	if( ! this->valid )
	{
		throw runtime_error("No valid pattern compiled");
	}

	vector<Match> ret(maxmatch+1);
	//ret.reserve(maxmatch+1);

	int res = regexec( & this->preg, line.c_str(), maxmatch, &ret[0], 0);

	if( res != REG_NOERROR && res != REG_NOMATCH )
	{
		throw runtime_error("Error doing pattern match");
	}


	if( res == REG_NOMATCH )
	{
		//printf("No match found\n");
		return {};
	}

	auto pend = remove_if( ret.begin(),ret.end(),
						   [](Regex::Match m)
							{
								return ( m.rm_so < 1 )  && ( m.rm_eo < 1);
							});
	ret.erase(pend, ret.end());

	return ret;
}

void Regex::Compile(const string &pattern, bool insensitive)
{
	if( this->valid )
	{
		this->Free();
	}
	int flags = REG_EXTENDED;

	if( insensitive )
	{
		flags |= REG_ICASE;
	}

	if( regcomp( &this->preg, pattern.c_str(), flags ) != 0 )
	{
		this->valid = false;
		throw runtime_error("Failed to compile pattern");
	}

	this->valid = true;
}

Regex::~Regex()
{
	if( this->valid )
	{
		this->Free();
	}
}

void Regex::Free()
{
	regfree( &this->preg );
	this->valid = false;
}

} // Namespace Utils
