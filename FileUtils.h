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

constexpr mode_t UserRead	= S_IRUSR;
constexpr mode_t UserWrite	= S_IWUSR;
constexpr mode_t UserExec	= S_IXUSR;

constexpr mode_t UserRWX	= UserRead | UserWrite | UserExec;
constexpr mode_t UserRX	= UserRead | UserExec;
constexpr mode_t UserRW		= UserRead | UserWrite;

constexpr mode_t GroupRead	= S_IRGRP;
constexpr mode_t GroupWrite	= S_IWGRP;
constexpr mode_t GroupExec	= S_IXGRP;

constexpr mode_t GroupRWX	= GroupRead | GroupWrite | GroupExec;
constexpr mode_t GroupRX	= GroupRead | GroupExec;
constexpr mode_t GroupRW	= GroupRead | GroupWrite;

constexpr mode_t OtherRead	= S_IROTH;
constexpr mode_t OtherWrite	= S_IWOTH;
constexpr mode_t OtherExec	= S_IXOTH;

constexpr mode_t OtherRWX	= OtherRead | OtherWrite | OtherExec;
constexpr mode_t OtherRX	= OtherRead | OtherExec;
constexpr mode_t OtherRW	= OtherRead | OtherWrite;

constexpr mode_t SetUserID	= S_ISUID;
constexpr mode_t SetGroupID	= S_ISGID;
constexpr mode_t Sticky		= S_ISVTX;

/**
 * @brief Size Return size of a file
 * @param path Path to file
 * @return size of file
 */
size_t Size(const std::string& path);

/**
 * @brief Copy make a copy of file
 * @param source Path to source file
 * @param destination Path to destination including filename
 */
void Copy(const std::string& source, const std::string& destination);

/**
 * @brief Write write string to file named with path
 * @param path Name and path of file
 * @param content String content to write
 * @param mode Octal creation mode
 */
void Write(const std::string& path, const std::string& content, mode_t mode);


/**
 * @brief SafeWrite same function as write but makes sure write succeds to tmp file
 *        before overwriting the destination
 *
 *        Note, tmp file will be created in same directory as destination file.
 *
 * @param path Where to write the file, path and name
 * @param content string to write as file contents
 * @param mode file permissions to use when creating file
 */
void SafeWrite(const std::string& path, const std::string& content, mode_t mode);


/**
 * @brief Write list of strings
 * @param path Where to write, path and filename
 * @param content list with strings to be written
 * @param mode File permission mode to create file with
 */
void Write(const std::string& path, const std::list<std::string>& content, mode_t mode);

/**
 * @brief Safely write list of strings, write to tempfile on success move to target
 * @param path Where to write, path and filename
 * @param content list with strings to be written
 * @param mode File permission mode to create file with
 */
void SafeWrite(const std::string& path, std::list<std::string>& content, mode_t mode);

/**
 * @brief Write buffer to file
 * @param path Where to write, path and filename
 * @param buf buffer with content to write
 * @param len lenght of buffer in bytes
 * @param mode File permission mode to create file with
 */
void Write(const std::string& path, const void* buf, size_t len, mode_t mode);

/**
 * @brief Safely write buffer to file, i.e. first write to tempfile and then rename
 * @param path Where to write, path and filename
 * @param buf buffer with content to write
 * @param len lenght of buffer in bytes
 * @param mode File permission mode to create file with
 */
void SafeWrite(const std::string& path, const void* buf, size_t len, mode_t mode);

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

void MkDir(const std::string& path, mode_t mode);
void MkPath(std::string path, mode_t mode);

/**
 * @brief GetPath return path excluding last part i.e. file
 *        aka dirname
 * @param s Path to return dirpath from
 * @return Directory path
 */
std::string GetPath(const std::string& s);

/**
 * @brief GetFileName return last part of path, i.e. filename
 *        aka basename
 * @param s Path to retrieve filename from
 * @return Filename
 */
std::string GetFileName(const std::string& s);

/**
 * @brief RealPath return the canonicalized absolute pathname
 * @param path
 * @return canonicalized absolute pathname
 */
std::string RealPath(const std::string& path);

std::list<std::string> Glob(const std::string& pattern);

void Delete(const std::string& path);

void Move(const std::string& s_path, const std::string& t_path);

}//NS File
}//NS Utils



#endif /* FILEUTILS_H_ */
