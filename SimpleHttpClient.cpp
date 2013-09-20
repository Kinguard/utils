/*
 * SimpleHttpClient.cpp
 *
 *  Created on: 26 jun 2012
 *      Author: tor
 */

#include "SimpleHttpClient.h"
#include "Socket.h"
#include "String.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std::placeholders;

namespace Utils {

using namespace Net;

const string SimpleHttpClient::delim="\r\n\r\n";

SimpleHttpClient::SimpleHttpClient():
		host(""),path(""),port(0), reqmethod(Http::GET), status(0)
{
	this->buffer.reserve(16*1024);

	// Setup default writer
	this->content.reserve(16*1024);
}

SimpleHttpClient::SimpleHttpClient(const string& host, const string& path, int port):
		host(host), path(path),port(port), reqmethod(Http::GET), status(0)
{
	this->buffer.reserve(16*1024);

	// Setup default writer
	this->content.reserve(16*1024);
}

void SimpleHttpClient::Get()
{
	if( port == 0 ){
		throw std::runtime_error("Missing port");
	}

	if( host == ""){
		throw std::runtime_error("Missing host");
	}

	if( path == "" ){
		this->path = "/";
	}

	TCPClientSocket s(host, port);

	this->WriteRequest(s);

	this->ReadHeaders(s);

	this->ReadBody(s);
}

void SimpleHttpClient::Head()
{
	if( port == 0 ){
		throw std::runtime_error("Missing port");
	}

	if( host == ""){
		throw std::runtime_error("Missing host");
	}

	if( path == "" ){
		this->path = "/";
	}

	TCPClientSocket s(host, port);

	this->reqmethod = Http::HEAD;
	this->WriteRequest(s);

	this->ReadHeaders(s);
}


void SimpleHttpClient::WriteRequest(Utils::TCPClientSocket& sock)
{
	stringstream request;

	switch( this->reqmethod )
	{
	case Http::GET:
		request << "GET "<< this->path <<" HTTP/1.1\r\n";
		break;
	case Http::HEAD:
		request << "HEAD "<< this->path <<" HTTP/1.1\r\n";
		break;
	default:
		throw std::runtime_error("Request method not implemented");
		break;
	}

	request << "HOST: "<< this->host <<":"<< this->port <<"\r\n";
	request << "Connection: close\r\n";
	request << "\r\n";

	sock.Write(request.str().c_str(), request.str().length());
}

void SimpleHttpClient::ReadHeaders(Utils::TCPClientSocket& sock)
{
	size_t rd = 0, found;
	do{
		rd += sock.AppendTo(this->buffer,50);
	}while(!(found = this->FindDelimiter()));

	string header(&this->buffer[0],found);
	header += "\r\n";
	list<string> headerlines = String::Split(header,"\r\n");

	//TODO: Merge splitted request headerlines, which spans multiple lines.

	// Get status line
	string status = headerlines.front();

	vector<string> sts;
	String::Split( status, sts );
	if( sts.size() > 1 )
	{
		this->httpversion = sts[0];
		this->status = atoi( sts[1].c_str() );
	}

	headerlines.pop_front();

	//TODO: parse status

	// Parse remaining headerlines
	for( auto line: headerlines){
		list<string> lvals = String::Split(line,":",2);
		if(lvals.size()>1){
			this->headers[String::ToUpper( lvals.front() )]=lvals.back();
		}
	}
#if 0
	for( auto val: this->headers){
		cout << "Key: ["<<val.first<<"] value ["<< val.second<<"]"<<endl;

	}
#endif
	// Remove header and try to read rest of request
	this->buffer.erase(
			this->buffer.begin(),
			this->buffer.begin() + found + SimpleHttpClient::delim.size());

}

void SimpleHttpClient::WriteData(vector<char>& v, size_t len)
{
	if( v.size() > 0 )
	{
		size_t cur_size = this->content.size();

		this->content.resize( cur_size + len );

		memcpy( &( this->content[cur_size] ), &(v[0]), len );

		v.erase(v.begin(), v.begin()+len);
		cout << "HTTP write "<<len<< " bytes. Buffer now: "<<v.size()<<endl;
	}
}


void SimpleHttpClient::ReadBody(Utils::TCPClientSocket& sock)
{
	// Find out how to parse body.
	// See RFC 2616 section 4.4
	//TODO: Implement multipart/byteranges?
	if( this->headers.find("TRANSFER-ENCODING") != this->headers.end() ){
		if( this->headers["TRANSFER-ENCODING"] == "chunked" ){
			// Do chunked read
			this->ReadChunked( sock );
		}else{
			//Unsupported mode, do error
			throw std::runtime_error(string("Unknown transfer encoding: ")+headers["TRANSFER-ENCODING"]);
		}

	}else if( this->headers.find("CONTENT-LENGTH") != this->headers.end() ){
		// Do read up to content len
		size_t to_read = atoll(this->headers["CONTENT-LENGTH"].c_str());

		// Make sure we have enough space
		this->ReserveStorage( to_read );

		// Remove part already read
		to_read -= this->buffer.size();

		// Write any initial data to store
		this->WriteData( this->buffer, this->buffer.size() );

		ssize_t rd = 0;
		do{
			rd = sock.Read( this->buffer );
			cout << "read "<<rd<<" bytes"<<endl;
			if( rd > 0 )
			{
				this->WriteData( this->buffer, this->buffer.size() );
			}
			to_read -= rd;
		}while( to_read > 0 && rd > 0);

	}else{
		// Assume connection close and read to the bitter end
		ssize_t rd = 0;
		do{
			rd = sock.AppendTo(this->content,4096);
		}while( rd > 0);

	}
}


bool SimpleHttpClient::readChunk() {
	const char* nl="\r\n";
	bool found = false;

	// Check for an initial \r\n and remove if present
	if( this->buffer.size() > 1  && this->buffer[0] == '\r' && this->buffer[1] == '\n' )
	{
		this->buffer.erase(this->buffer.begin(), this->buffer.begin()+2 );
		cout << "Erasing possible trailing newline. Size now is: "<<this->buffer.size()<<endl;
	}

	auto it = search( this->buffer.begin(), this->buffer.end(), nl, nl+2);

	if( it != this->buffer.end() )
	{
		ssize_t pos = int(it-this->buffer.begin());
		string s(&this->buffer[0],&this->buffer[0]+pos+2 /* =strlen(nl) */ );

		size_t clen;
		stringstream ss(s);
		if ( ss >> std::setbase(16) >> clen )
		{
			// Found chunk length
			cout << " Chunk length: "<<clen<<endl;

			// Check if we have whole chunk in buffer
			if( this->buffer.size() >= ( clen + s.length() )  ){

				// Erase line from buffer.
				this->buffer.erase(this->buffer.begin(), this->buffer.begin() + s.length() );

				this->WriteData( this->buffer, clen );

				found = true;
			}
		}else{
			// Error!
			//cout << "Got chunk len  "<<s;
			HexDump(&this->buffer[0], this->buffer.size());
			throw std::runtime_error("Failed to find chunk length");
		}
	}

	return found;
}

void SimpleHttpClient::ReadChunked(Utils::TCPClientSocket& sock)
{
	ssize_t rd = 0;

	do
	{
		rd = sock.AppendTo( this->buffer, 4096 );

		try
		{
			// Read out as many chunks as possible
			while( this->readChunk() );

		}
		catch( std::runtime_error& err )
		{
			cout << "Got error ["<< err.what()  << "]" << endl;
			rd = 0;
		}


	}while( rd > 0 && this->buffer.size() < 0x20000 );

}


int SimpleHttpClient::getStatus() const {
	return this->status;
}

const map<string, string>& SimpleHttpClient::Headers(void)
{
	return this->headers;
}

void SimpleHttpClient::ReserveStorage(size_t sz)
{
	if( this->content.capacity() < sz){
		this->content.reserve(sz);
	}
}


size_t SimpleHttpClient::FindDelimiter()
{

	auto fnd = search(this->buffer.begin(), this->buffer.end(),
			SimpleHttpClient::delim.begin(),
			SimpleHttpClient::delim.end());

	size_t pos = int(fnd - this->buffer.begin());

	return pos != this->buffer.size() ? pos : 0;
}

void SimpleHttpClient::setHost(string host) {
	this->host = host;
}

string SimpleHttpClient::getPath() const {
	return path;
}

void SimpleHttpClient::setPath(string path) {
	this->path = path;
}
int SimpleHttpClient::getPort() const {
	return port;
}

void SimpleHttpClient::setPort(int port) {
	this->port = port;
}

string SimpleHttpClient::getHost() const {
	return host;
}

vector<char> SimpleHttpClient::Content(void) {
	return this->content;
}

string SimpleHttpClient::Header(const char* key) {
	return this->headers[key];
}


SimpleHttpClient::~SimpleHttpClient() {

}

} /* namespace Utils */
