/*
 * SimpleHttpClient.h
 *
 *  Created on: 26 jun 2012
 *      Author: tor
 */

#ifndef SIMPLEHTTPCLIENT_H_
#define SIMPLEHTTPCLIENT_H_

#include <vector>
#include <string>
#include <map>
#include "Socket.h"

using namespace std;

namespace Utils {

class SimpleHttpClient {
public:
	SimpleHttpClient();
	SimpleHttpClient(const string& host, const string& url, int port=80);
	void Fetch();
	virtual ~SimpleHttpClient();

	string getHost() const;

	void setHost(string host);

	string getPath() const;

	void setPath(string path);

	int getPort() const;

	void setPort(int port);

	string Header(const char* key);

	vector<char> Content(void);

private:
	void WriteRequest(Utils::TCPClient& sock);
	void ReadHeaders(Utils::TCPClient& sock);
	void ReadBody(Utils::TCPClient& sock);
	size_t FindDelimiter();

	string host;
	string path;
	int port;
	map<string,string> headers;
	//TODO: Should this perhaps be a refcounted smartptr?
	vector<char> content;
	static const string delim;
};

}/* namespace Utils */
#endif /* SIMPLEHTTPCLIENT_H_ */
