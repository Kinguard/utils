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

#ifndef SIMPLEHTTPCLIENT_H_
#define SIMPLEHTTPCLIENT_H_

#include <functional>
#include <vector>
#include <string>
#include <map>
#include "Socket.h"

using namespace std;

namespace Utils {

namespace Http {
	typedef enum {
		OPTIONS,
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		TRACE,
		CONNECT
	} RequestMethod;
}

class SimpleHttpClient {
public:
	SimpleHttpClient();
	SimpleHttpClient(const string& host, const string& url, int port=80);
	virtual ~SimpleHttpClient();

	// Request methods
	void Get();
	void Head();

	// Configure object
	string getHost() const;
	void setHost(string host);

	string getPath() const;
	void setPath(string path);

	int getPort() const;
	void setPort(int port);

	// Local implementation of back storage
	virtual void WriteData(vector<char>& v, size_t len);
	virtual void ReserveStorage(size_t sz);

	// Retrieve data
	string Header(const char* key);
	const map<string,string>& Headers(void);

	int getStatus() const;

	vector<char> Content(void);

private:
	void WriteRequest(Utils::Net::TCPClientSocket& sock);

	void ReadHeaders(Utils::Net::TCPClientSocket& sock);
	void ReadBody(Utils::Net::TCPClientSocket& sock);
	void ReadChunked(Utils::Net::TCPClientSocket& sock);
	bool readChunk();

	size_t FindDelimiter();

	string host;
	string path;
	int port;
	Http::RequestMethod reqmethod;

	map<string,string> headers;
	string httpversion;
	int status;
	//TODO: Should this perhaps be a refcounted smartptr?
	vector<char> content; // Store read content
	vector<char> buffer; // Temporary buffer for read data.
	static const string delim;
};

}/* namespace Utils */
#endif /* SIMPLEHTTPCLIENT_H_ */
