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

namespace Utils {

const string SimpleHttpClient::delim="\r\n\r\n";

SimpleHttpClient::SimpleHttpClient(): host(""),path(""),port(0)
{
	content.reserve(16384);
}

SimpleHttpClient::SimpleHttpClient(const string& host, const string& path, int port):
		host(host), path(path),port(port)
{
}

void SimpleHttpClient::Fetch()
{
	if( port == 0 ){
		throw std::runtime_error("Missing port");
	}

	if( host == ""){
		throw std::runtime_error("Missing host");
	}

	if( path == "" ){
		throw std::runtime_error("Missing path");
	}

	TCPClient s(host, port);

	this->WriteRequest(s);

	this->ReadHeaders(s);

	this->ReadBody(s);
}

void SimpleHttpClient::WriteRequest(Utils::TCPClient& sock)
{
	stringstream request;
	request << "GET "<< this->path <<" HTTP/1.1\r\n";
	request << "HOST: "<< this->host <<":"<< this->port <<"\r\n";
	request << "Connection: close\r\n";
	request << "\r\n";

	sock.Write(request.str().c_str(), request.str().length());
}

void SimpleHttpClient::ReadHeaders(Utils::TCPClient& sock)
{
	size_t rd = 0, found;
	do{
		rd += sock.AppendTo(this->content,50);
	}while(!(found = this->FindDelimiter()));

	string header(&this->content[0],found);
	header += "\r\n";
	list<string> headerlines = String::Split(header,"\r\n");

	//TODO: Merge splitted request headerlines, which spans multiple lines.

	// Get status line
	string status = headerlines.front();
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
	this->content.erase(
			this->content.begin(),
			this->content.begin() + found + SimpleHttpClient::delim.size());

}

void SimpleHttpClient::ReadBody(Utils::TCPClient& sock)
{
	// Find out how to parse body.
	// See RFC 2616 section 4.4
	//TODO: Implement multipart/byteranges?
	if( this->headers.find("TRANSFER-ENCODING") != this->headers.end() ){
		if( this->headers["TRANSFER-ENCODING"] == "chunked" ){
			// Do chunked read
			throw std::runtime_error("Chunked read not implemented yet");
		}else{
			//Unsupported mode, do error
			throw std::runtime_error(string("Unknown transfer encoding: ")+headers["TRANSFER-ENCODING"]);
		}

	}else if( this->headers.find("CONTENT-LENGTH") != this->headers.end() ){
		// Do read up to content len
		size_t to_read = atoll(this->headers["CONTENT-LENGTH"].c_str());

		// Make sure we have enough space
		if( this->content.capacity() < to_read){
			this->content.reserve(to_read);
		}

		// Remove part already read
		to_read -= this->content.size();
		ssize_t rd = 0;
		do{
			rd = sock.AppendTo(this->content,to_read);
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

size_t SimpleHttpClient::FindDelimiter()
{

	auto fnd = search(this->content.begin(), this->content.end(),
			SimpleHttpClient::delim.begin(),
			SimpleHttpClient::delim.end());

	size_t pos = int(fnd - this->content.begin());

	return pos != this->content.size() ? pos : 0;
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
