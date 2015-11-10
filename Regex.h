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

#ifndef REGEX_H
#define REGEX_H

#include <regex.h>
#include <sys/types.h>

#include <string>
#include <vector>

using namespace std;

namespace Utils {

class Regex
{
public:

	typedef regmatch_t Match;

	Regex();
	Regex( const string& pattern, bool insensitive = false);

	vector<Match> DoMatch(const string& line, int maxmatch = 10);

	void Compile( const string& pattern, bool insensitive = false );

	virtual ~Regex();
private:

	void Free();

	bool valid;
	regex_t preg;
};

} /* namespace Utils */
#endif // REGEX_H
