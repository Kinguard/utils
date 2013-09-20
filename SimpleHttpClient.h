/*
 * SimpleHttpClient.h
 *
 *  Created on: 26 jun 2012
 *      Author: tor
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
