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

/**
 * @brief The ConfigFile class
 *        A simple configuration handler to read and write key value
 *        config files.
 *
 * Config files are line based key value pairs separated by =
 * Lines starting with # is regarded as comments and ignored.
 *
 */
class ConfigFile: public map<string, string>
{
public:
	/**
	 * @brief ConfigFile
	 * @param filename - file to operate on
	 */
	ConfigFile(const string& filename);

	/**
	 * @brief ValueOrDefault - return key or a default value
	 * @param key - Key to lookup
	 * @param defval - Default value to use if key not present
	 * @return Value of key or default value
	 */
	string ValueOrDefault(const string& key, const string& defval="");

	/**
	 * @brief Dump - write current content of config to stdout
	 */
	void Dump();

	/**
	 * @brief Sync - write back contents to file
	 * @param create - should file be created if not existant
	 * @param mode - file mod to use when writing
	 */
	void Sync(bool create = true, mode_t mode=0640);

	virtual ~ConfigFile();
private:
	string ToString();
	string filename;
};

/**
 * @brief The IniFile class
 *        A simple inifile parser with some config posibilities.
 *
 * Inifile can either be read from file or constructed from a list of strings.
 * Key value delimiter and comment character is configurable
 *
 * Add values by assigning to the object. Retrieve values either by using
 * Value, ValueOrDefault accessors or by indexing directly
 *
 * Example:
 *
 * IniFile i("/path/to/inifile.ini")
 * i["section"]["key"] = "Value";
 *
 * string val = i.Value("section","key","default value");
 *
 * i.UseSection("section");
 * string val = i.ValueOrDefault("key","default value");
 *
 * string val = i["section"]["value"];
 */
class IniFile: public map<string, map<string, string>>
{
public:
	/**
	 * @brief IniFile - create an IniFile object from a possibly existant file
	 *        if file not present name is saved and possibly used at save.
	 * @param filename - Filename to use for IniFile
	 * @param delim - delimiter between key and value on line
	 * @param comment - comment character to comment line
	 */

	IniFile(const string& filename, string  delim = "=", char comment='#');
	/**
	 * @brief IniFile - create IniFile object from list of lines
	 * @param lines - List with lines to use as data
	 * @param delim - delimiter between key and value on line
	 * @param comment - comment character to comment line
	 */
	IniFile(const list<string>& lines, string  delim = "=", char comment='#');

	/**
	 * @brief UseSection - set current section to operate on
	 * @param section - Section to use from now
	 */
	void UseSection( const string& section);

	/**
	 * @brief Value - retrieve value from IniFile
	 * @param section - section to retrieve from
	 * @param key - Key to retrieve
	 * @param defval - Default value if key not present
	 * @return value of key or default value
	 */
	string Value(const string& section, const string& key, const string& defval="");

	/**
	 * @brief ValueOrDefault - use current section and get value of key
	 * @param key - Key to retrieve value of
	 * @param defval - Default value to use if key not present
	 * @return value of key or default
	 */
	string ValueOrDefault(const string& key, const string& defval="");


	/**
	 * @brief Save - save current IniFile to filename
	 * @param filename - Filename to use when saving, if empty original filenamn is used
	 * @param create - Should file be created if non existant
	 * @param mode - Mode of file when writing
	 */
	void Save(const string& filename="", bool create = true, mode_t mode=0640);

	/**
	 * @brief Dump - write current content of config to stdout
	 */
	void Dump();

	virtual ~IniFile();
private:
	void ParseInput(const list<string> &rows);
	string ToString();
	string delimiter;
	char comment;
	string filename;
	string currsection;
};

}
#endif // CONFIGFILE_H
