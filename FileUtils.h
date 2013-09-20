/*
 * FileUtils.h
 *
 *  Created on: Jul 8, 2012
 *      Author: tor
 */

#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <string>
#include <list>

#include "Exceptions.h"

#include <sys/types.h>

namespace Utils{
namespace File{

bool FileExists(const std::string& path);
bool DirExists(const std::string& path);
bool SocketExists(const std::string& path);
std::string GetContentAsString(const std::string& path);
std::list<std::string> GetContent(const std::string& path);

void Write(const std::string& path, const std::string& content, mode_t mode);
void Write(const std::string& path, std::list<std::string>& content, mode_t mode);

void MkDir(const std::string& path, mode_t mode);
void MkPath(std::string path, mode_t mode);

std::list<std::string> Glob(const std::string& pattern);

void Delete(const std::string& path);

}//NS File
}//NS Utils



#endif /* FILEUTILS_H_ */
