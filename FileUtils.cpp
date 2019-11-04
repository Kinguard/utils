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

#include "FileUtils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <glob.h>

#include <ext/stdio_filebuf.h>
#include <fstream>
#include <tuple>

using namespace std;

static mode_t do_stat(const std::string& path )
{
	struct stat st;
	if(lstat(path.c_str(),&st)){
		if( errno == ENOENT ){
			return false;
		}
		throw Utils::ErrnoException("Failed to check file");
	}
	return st.st_mode;
}

bool Utils::File::FileExists(const std::string& path)
{
	return S_ISREG( do_stat(path) );
}

bool Utils::File::LinkExists(const string &path)
{
	return S_ISLNK( do_stat(path) );
}

bool Utils::File::DirExists(const std::string& path)
{
	return S_ISDIR( do_stat(path) );
}

bool
Utils::File::SocketExists ( const std::string& path )
{
	return S_ISSOCK( do_stat(path) );
}


std::string Utils::File::GetContentAsString(const std::string& path, bool keeplinending)
{
	string ret;
	string linend = keeplinending?"\n":"";

	ifstream in(path.c_str(),ifstream::in);
	string line;
	while(in.good()){
		getline(in,line);
		if(line.size()>0 || !in.eof()){
			ret+=line+linend;
		}
	}
	in.close();
	return ret;
}

std::list<std::string> Utils::File::GetContent(const std::string& path)
{
	list<string> ret;
	ifstream in(path.c_str(),ifstream::in);
	string line;
	while(in.good()){
		getline(in,line);
		if(line.size()>0 || !in.eof()){
			ret.push_back(line);
		}
	}
	in.close();
	return ret;
}

static int do_open(const string& path, mode_t mode, bool write = true)
{
	int fd, flags = write?O_WRONLY|O_CREAT|O_TRUNC:O_RDONLY;

	if((fd=open(path.c_str(),flags,mode))<0){
		throw Utils::ErrnoException("Unable to open file '"+path+"' for " + string(write?"writing":"reading"));
	}
	return fd;
}

static void do_write(int fd, const void* buf, size_t len)
{
	size_t write_total = 0, to_write = len;

	while ( write_total < to_write )
	{
		ssize_t written = write(fd, (void*)( (size_t)buf + write_total), to_write - write_total );
		if( written < 0 )
		{
			throw Utils::ErrnoException("Write failed");
		}
		write_total += static_cast<size_t>(written);
	}
}


static void do_write(int fd, const std::string& content)
{
	do_write( fd, content.c_str(), content.size());
}

void Utils::File::Write(const std::string& path, const std::string& content,
		mode_t mode)
{
	int fd = do_open(path, mode);

	do_write( fd, content );

	close(fd);
}

static void do_write(int fd, const std::list<std::string>& content)
{
	for(const auto& line: content)
	{
		do_write(fd, line.c_str(), line.size());
	}
}

void Utils::File::Write(const std::string& path,
		const std::list<string> &content, mode_t mode)
{
	int fd = do_open(path, mode);

	do_write(fd, content);

	close(fd);
}

void
Utils::File::Write ( const std::string& path, const void* buf, size_t len,
		mode_t mode )
{
	int fd = do_open(path, mode);

	do_write( fd, buf, len);

	close(fd);
}

void
Utils::File::Read ( const std::string& path, const void* buf, size_t len )
{
	int fd = do_open(path, 0, false);

	size_t read_total = 0;
	ssize_t bytes_read;
	do
	{
		bytes_read = read(fd, (void*)((size_t)buf + read_total), len - read_total);
		if(bytes_read < 0 )
		{
			throw ErrnoException("Failed to read file '"+path+"'");
		}
		if( bytes_read > 0 )
		{
			read_total += static_cast<size_t>(bytes_read);
		}

	}while( bytes_read>0);

	close(fd);
}


void Utils::File::MkDir(const std::string& path, mode_t mode)
{
	if(mkdir(path.c_str(),mode)){
		throw ErrnoException("Failed to create directory");
	}
}

void Utils::File::MkPath(std::string path, mode_t mode)
{
	if(path[path.length()-1]!='/'){
		path+="/";
	}

	string::size_type pos=0;
	while((pos=path.find("/",pos))!=string::npos){
		if(pos!=0){
			if( ! Utils::File::DirExists( path.substr(0,pos) ) ){
				MkDir(path.substr(0,pos).c_str(),mode);
			}
		}
		pos++;
	}
}

std::list<std::string> Utils::File::Glob(const std::string& pattern)
{
	glob_t gb;
	int ret;

	if((ret=glob(pattern.c_str(),GLOB_NOSORT,nullptr,&gb))){
		if(ret!=GLOB_NOMATCH){
			throw ErrnoException("Failed to glob");
		}
	}

	list<string> res;
	for(unsigned int i=0;i<gb.gl_pathc;i++){
			res.push_back(gb.gl_pathv[i]);
	}

	globfree(&gb);

	return res;
}

void Utils::File::Delete(const std::string& path)
{
	/* Socket, file or link */
	if( FileExists( path ) || SocketExists( path ) || LinkExists( path ) ) {
		if( unlink( path.c_str() ) < 0 ){
			throw ErrnoException("Failed to delete file");
		}
	}else if( Utils::File::DirExists( path )){
		if( rmdir( path.c_str() ) < 0 ){
			throw ErrnoException("Failed to delete directory");
		}
	}else{
		throw std::runtime_error("Unknown media to delete");
	}
}

void Utils::File::Move(const std::string& s_path,const std::string& t_path)
{
    /* File */
    if( FileExists( s_path ) ) {
        if( FileExists( t_path ) ) {
            throw ErrnoException("Target already exists");
        }
        else
        {
            if( rename( s_path.c_str(), t_path.c_str() ) < 0 ){
                throw ErrnoException("Failed to move file");
            }
        }
    /* Dir */
    }else if( Utils::File::DirExists( s_path )){
        if( DirExists( t_path ) ) {
            throw ErrnoException("Target already exists");
        }
        else
        {
            if( rename( s_path.c_str(), t_path.c_str() ) < 0 ){
                throw ErrnoException("Failed to move dir");
            }
        }
    }else{
        throw std::runtime_error("Unknown media to move");
    }
}

string Utils::File::GetPath(const string &s)
{
	string::size_type pos;

	if( ( pos = s.find_last_of("/") ) != string::npos )
	{
		return s.substr(0 ,pos );
	}
	else
	{
		return s;
	}
}

string Utils::File::GetFileName(const string &s)
{
	string::size_type pos;

	if( ( pos = s.find_last_of("/") ) != string::npos )
	{
		return s.substr( pos + 1 );
	}
	else
	{
		return s;
	}
}

string Utils::File::RealPath(const string &path)
{
	char *rcpath = realpath( path.c_str(), nullptr);

	if( !rcpath )
	{
		throw ErrnoException("Realpath failed");
	}
	string rpath(rcpath);

	free( rcpath );

	return rpath;
}


static tuple<int,string> do_mkstemp(const string& path)
{
	string tmppath = Utils::File::GetPath(path)+"/tmpfileXXXXXX";
	char *ctmp = new char[ tmppath.length()+1];

	std::strcpy(ctmp, tmppath.c_str() );
	int fd = mkstemp(ctmp);

	if( fd < 0 )
	{
		throw Utils::ErrnoException("Unable to open file '"+path+"' for writing");
	}

	tmppath = ctmp;
	delete [] ctmp;

	return make_tuple(fd, tmppath);
}

void Utils::File::SafeWrite(const string &path, const string &content, mode_t mode)
{
	int fd;
	string tmppath;

	tie(fd, tmppath) = do_mkstemp(path);
	do_write(fd, content );
	close(fd);

	chmod(tmppath.c_str(), mode);

	// Try delete destination
	try {
		File::Delete(path);
	} catch (std::runtime_error& err) {
		(void)err;
	}

	File::Move(tmppath, path);
}


void Utils::File::SafeWrite(const string &path, std::list<string> &content, mode_t mode)
{
	int fd;
	string tmppath;

	tie(fd, tmppath) = do_mkstemp(path);
	do_write(fd, content);
	close(fd);

	chmod(tmppath.c_str(), mode);

	// Try delete destination
	try {
		File::Delete(path);
	} catch (std::runtime_error& err) {
		(void)err;
	}

	File::Move(tmppath, path);
}

void Utils::File::SafeWrite(const string &path, const void *buf, size_t len, mode_t mode)
{
	int fd;
	string tmppath;

	tie(fd, tmppath) = do_mkstemp(path);
	do_write(fd, buf, len);
	close(fd);

	chmod(tmppath.c_str(), mode);

	// Try delete destination
	try {
		File::Delete(path);
	} catch (std::runtime_error& err) {
		(void)err;
	}

	File::Move(tmppath, path);
}
