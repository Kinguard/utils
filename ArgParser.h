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

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <map>
#include <vector>
#include <list>

#include <getopt.h>

using namespace std;

namespace Utils
{

class Option
{
public:
	enum ArgType {
		ArgNone,
		ArgRequired,
		ArgOptional
		};

	Option(char short_desc, const string& long_desc, enum ArgType type,
			const string& default_val="", const string& helptext=""):
		short_desc(short_desc), long_desc(long_desc), type(type),
		default_val(default_val), helptext(helptext)
	{
	}

	virtual ~Option(){}

	enum ArgType type;
	char short_desc;
	string long_desc;
	string default_val;
	string helptext;
};

class ArgParser: public map<string,string>
{
public:
	ArgParser();

	void AddOption(const Option& opt);
	void AddOptions( const list<Option>& l);

	bool Parse(int argc, char ** const argv);

	bool HasReminder();
	list<string> GetReminder();

	string GetHelptext();

	void _Dump(void);

	virtual ~ArgParser();

private:
	int FindIndex(char c);

	vector<Option> opts;
	string shortargs;
	vector<struct option> longopts;
	list<string> reminder;
};

} // NS Utils
#endif // ARGPARSER_H
