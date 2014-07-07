#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <map>
#include <string>

using namespace std;

namespace Utils{


class ConfigFile: public map<string, string>
{
public:
	ConfigFile(const string& filename);

	string ValueOrDefault(const string& key, const string& defval="");

	void Dump();

	void Sync(bool create = true, mode_t mode=0640);

	virtual ~ConfigFile();
private:
	string ToString();
	string filename;
};

}
#endif // CONFIGFILE_H
