
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
		string line =  String::Trimmed(row, " ");

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
			String::Trimmed(keyval.front(), " "),
			String::Trimmed(keyval.back(), " ") ));
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

} // Namespace Utils
