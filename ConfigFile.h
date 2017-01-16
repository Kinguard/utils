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

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <map>
#include <list>
#include <string>

using namespace std;

namespace Utils{


class ConfigFile: public map<string, string>
{
public:
	ConfigFile(const string& filename);

	string ValueOrDefault(const string& key, const string& defval="");

	void Dump();

	void Sync(bool create = true, mode_t mode=0640);

	virtual ~ConfigFile();
private:
	string ToString();
	string filename;
};

class IniFile: public map<string, map<string, string>>
{
public:
	IniFile(const string& filename, const string& delim = "=", char comment='#');
	IniFile(const list<string>& lines, const string& delim = "=", char comment='#');

	void UseSection( const string& section);

	string Value(const string& section, const string& key, const string& defval="");
	string ValueOrDefault(const string& key, const string& defval="");

	void Dump();

	virtual ~IniFile();
private:
	void ParseInput(const list<string> &rows);
	string delimiter;
	char comment;
	string currsection;
};

}
#endif // CONFIGFILE_H
