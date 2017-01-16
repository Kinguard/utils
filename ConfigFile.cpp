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

#include <list>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#include "Exceptions.h"
#include "FileUtils.h"
#include "String.h"


#include "ConfigFile.h"

namespace Utils
{

ConfigFile::ConfigFile(const string &filename): filename(filename)
{
	list<string> rows = File::GetContent(filename);

	for( auto row: rows)
	{
		string line =  String::Trimmed(row, " \t");

		if( line == "" || line.front() =='#' )
		{
			continue;
		}

		list<string> keyval = String::Split(line, "=");
		if( keyval.size() != 2 )
		{
			continue;
		}

		this->insert(pair<string,string>(
			String::Trimmed(keyval.front(), " \t"),
			String::Trimmed(keyval.back(), " \t") ));
	}
}

string ConfigFile::ValueOrDefault(const string &key, const string &defval)
{
	if( this->find(key) != this->end() )
	{
		return this->operator[](key);
	}

	return defval;
}

void ConfigFile::Dump()
{
	printf("%s\n", this->ToString().c_str() );
}

void ConfigFile::Sync(bool create, mode_t mode)
{
	if( !create && !File::FileExists( this->filename ) )
	{
		throw runtime_error("File does not exist and creation disallowed");
	}

	if( ! File::FileExists(this->filename) )
	{
		File::Write(this->filename, "", mode);
	}

	struct stat st;
	if( stat(this->filename.c_str(), &st) < 0 )
	{
		throw ErrnoException("Failed to stat file");
	}

	string tmpfile=this->filename + ".new";

	File::Write(tmpfile, this->ToString(), st.st_mode);

	if( rename( tmpfile.c_str(), this->filename.c_str() ) < 0 )
	{
		throw ErrnoException("Failed to rename config file");
	}

}


ConfigFile::~ConfigFile()
{

}

string ConfigFile::ToString()
{
	stringstream ss;

	for(auto it = this->cbegin(); it != this->end(); it++)
	{
		ss << (*it).first << "=" << (*it).second <<endl;
	}
	return ss.str();
}
/*
 *
 * Inifile implementation
 *
 */
IniFile::IniFile(const string &filename, const string &delim, char comment):
	delimiter(delim),
	comment(comment),
	filename(filename),
	currsection("")
{
	list<string> rows = File::GetContent(filename);

	this->ParseInput( rows );
}

IniFile::IniFile(const list<string> &lines, const string &delim, char comment):

	delimiter(delim),
	comment(comment),
	filename(""),
	currsection("")
{
	this->ParseInput( lines );
}

void IniFile::UseSection(const string &section)
{
	this->currsection = section;
}

string IniFile::Value(const string &section, const string &key, const string &defval)
{
	if( this->find(section) != this->end() )
	{
		if( this->operator [](section).find(key) != this->operator [](section).end())
		{
			return this->operator[](section).operator[](key);
		}
	}

	return defval;

}

string IniFile::ValueOrDefault(const string &key, const string &defval)
{
	return this->Value(this->currsection, key, defval);
}

void IniFile::Save(const string &filename, bool create, mode_t mode)
{
	if( filename == "" && this->filename == "" )
	{
		throw std::runtime_error("IniFile: missing filename for save");
	}

	string outname = filename == "" ? this->filename : filename;

	if( !create && !File::FileExists( outname ) )
	{
		throw runtime_error("File does not exist and creation disallowed");
	}

	if( ! File::FileExists( outname ) )
	{
		File::Write( outname, "", mode);
	}

	struct stat st;
	if( stat(outname.c_str(), &st) < 0 )
	{
		throw ErrnoException("Failed to stat file");
	}

	string tmpfile=outname + ".new";

	File::Write(tmpfile, this->ToString(), st.st_mode);

	if( rename( tmpfile.c_str(), outname.c_str() ) < 0 )
	{
		throw ErrnoException("Failed to rename config file");
	}
}

void IniFile::Dump()
{
	printf("%s\n", this->ToString().c_str() );
}

IniFile::~IniFile()
{

}

void IniFile::ParseInput(const list<string> &rows)
{
	for( auto row: rows)
	{
		string line =  String::Trimmed(row, " \t");

		if( line == "" || line.front() == this->comment )
		{
			continue;
		}

		if( line.front() == '[' && line.back() == ']')
		{
			// Got a "new" section
			this->currsection = String::Trimmed( line.substr(1,line.size()-2), " \t");
			continue;
		}

		list<string> keyval = String::Split(line, this->delimiter.c_str());
		if( keyval.size() != 2 )
		{
			continue;
		}

		if( this->currsection == "" )
		{
			//throw std::runtime_error("Iniparser: Malformed input, no section to add key value to");
		}
		this->operator [](this->currsection).insert( pair<string,string>(
					String::Trimmed(keyval.front(), " \t"),
					String::Trimmed(keyval.back(), " \t") ));
	}

}

string IniFile::ToString()
{
	stringstream ss;

	for( auto iT = this->cbegin(); iT != this->end(); iT++)
	{
		ss << "[" << iT->first <<"]" << endl;
		for( auto iiT = iT->second.cbegin(); iiT != iT->second.end(); iiT++)
		{
			ss << iiT->first << this->delimiter << iiT->second << endl;
		}
	}

	return ss.str();
}

} // Namespace Utils
