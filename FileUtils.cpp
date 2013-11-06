/*
 * FileUtils.cpp
 *
 *  Created on: Jul 8, 2012
 *      Author: tor
 */

#include "FileUtils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <glob.h>

#include <ext/stdio_filebuf.h>
#include <fstream>

using namespace std;

static bool do_stat(const std::string& path,mode_t mode )
{
	struct stat st;
	if(stat(path.c_str(),&st)){
		if( errno == ENOENT ){
			return false;
		}
		throw Utils::ErrnoException("Failed to check file");
	}
	return ((((st.st_mode)) & 0170000) & (mode));
}

bool Utils::File::FileExists(const std::string& path)
{
	return do_stat(path, S_IFREG);
}

bool Utils::File::DirExists(const std::string& path)
{
	return do_stat(path, S_IFDIR);
}

bool
Utils::File::SocketExists ( const std::string& path )
{
	return do_stat(path, S_IFSOCK);
}


std::string Utils::File::GetContentAsString(const std::string& path)
{
	string ret;
	ifstream in(path.c_str(),ifstream::in);
	string line;
	while(in.good()){
		getline(in,line);
		if(line.size()>0 || !in.eof()){
			ret+=line;
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

void Utils::File::Write(const std::string& path, const std::string& content,
		mode_t mode)
{
	int fd;
	if((fd=open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC,mode))<0){
		throw ErrnoException("Unable to open file '"+path+"' for writing");
	}

	{
		__gnu_cxx::stdio_filebuf<char> fb(fd,std::ios_base::out);
		iostream of(&fb);

		of<<content<<flush;
	}

	close(fd);
}

void Utils::File::Write(const std::string& path,
		std::list<std::string>& content, mode_t mode)
{
	int fd;
	if((fd=open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC,mode))<0){
		throw ErrnoException("Unable to open file for writing");
	}

	{
		__gnu_cxx::stdio_filebuf<char> fb(fd,std::ios_base::out);
		iostream of(&fb);

		for(list<string>::iterator sIt=content.begin();sIt!=content.end();sIt++){
			of << *sIt;
		}

		of<<flush;
	}

	close(fd);
}

void
Utils::File::Write ( const std::string& path, const void* buf, size_t len,
		mode_t mode )
{
	int fd;
	if((fd=open(path.c_str(),O_WRONLY|O_CREAT|O_TRUNC,mode))<0){
		throw ErrnoException("Unable to open file for writing");
	}
	size_t write_total = 0, to_write = len;

	while ( write_total < to_write )
	{
		ssize_t written = write(fd, (void*)((size_t)buf + write_total), to_write - write_total );
		if( written < 0 )
		{
			throw ErrnoException("Write failed");
		}
		write_total += written;
	}

	close(fd);
}


#if 0
void
Utils::File::Read ( const std::string& path, std::vector<unsigned char>& data )
{
	struct stat st;
	if(stat(path.c_str(),&st)){
		throw Utils::ErrnoException("Failed to stat file");
	}
	// Make sure we have room for data
	data.resize(st.st_size);

	int fd;
	if((fd=open(path.c_str(),O_RDONLY))<0){
		throw ErrnoException("Unable to open file '"+path+"' for reading");
	}

	size_t read_total = 0;
	ssize_t bytes_read;
	do
	{
		bytes_read = read(fd, &data[read_total], st.st_size - read_total);
		if(bytes_read < 0 )
		{
			throw ErrnoException("Failed to read file '"+path+"'");
		}
		if( bytes_read > 0 )
		{
			read_total += bytes_read;
		}

	}while( bytes_read>0);

	close(fd);
}
#endif

void
Utils::File::Read ( const std::string& path, const void* buf, size_t len )
{
	int fd;
	if((fd=open(path.c_str(),O_RDONLY))<0){
		throw ErrnoException("Unable to open file '"+path+"' for reading");
	}

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
			read_total += bytes_read;
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

	if((ret=glob(pattern.c_str(),GLOB_NOSORT,NULL,&gb))){
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
	if( do_stat(path, S_IFREG|S_IFSOCK|S_IFLNK) ){
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
