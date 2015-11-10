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

#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <string>
#include <vector>
#include <list>

#include "Exceptions.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Utils{
namespace File{

bool FileExists(const std::string& path);
bool LinkExists(const std::string& path);
bool DirExists(const std::string& path);
bool SocketExists(const std::string& path);
std::string GetContentAsString(const std::string& path, bool keeplinending=false);
std::list<std::string> GetContent(const std::string& path);

void Write(const std::string& path, const std::string& content, mode_t mode);
void Write(const std::string& path, std::list<std::string>& content, mode_t mode);

void Write(const std::string& path, const void* buf, size_t len, mode_t mode);

template <class T>
void WriteVector(const std::string& path, const T& v, mode_t mode)
{
	Write(path, &v[0], v.size(), mode );
}

void Read(const std::string& path, const void* buf, size_t len);

template <class T>
void ReadVector( const std::string& path, T& v)
{
	struct stat st;
	if(stat(path.c_str(),&st)){
		throw Utils::ErrnoException("Failed to stat file");
	}
	// Make sure we have room for data
	v.resize(st.st_size);

	Read(path, &v[0], st.st_size);
}
//void Read(const std::string& path, std::vector<unsigned char>& data);

void MkDir(const std::string& path, mode_t mode);
void MkPath(std::string path, mode_t mode);

/* Aka, dirname */
std::string GetPath(const std::string& s);
/* Aka, basename */
std::string GetFileName(const std::string& s);


std::list<std::string> Glob(const std::string& pattern);

void Delete(const std::string& path);

}//NS File
}//NS Utils



#endif /* FILEUTILS_H_ */
