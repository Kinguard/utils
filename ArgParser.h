#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <map>
#include <vector>
#include <list>

#include <getopt.h>

using namespace std;

namespace Utils
{

class Option
{
public:
	enum ArgType {
		ArgNone,
		ArgRequired,
		ArgOptional
		};

	Option(char short_desc, const string& long_desc, enum ArgType type,
			const string& default_val="", const string& helptext=""):
		short_desc(short_desc), long_desc(long_desc), type(type),
		default_val(default_val), helptext(helptext)
	{
	}

	virtual ~Option(){}

	enum ArgType type;
	char short_desc;
	string long_desc;
	string default_val;
	string helptext;
};

class ArgParser: public map<string,string>
{
public:
	ArgParser();

	void AddOption(const Option& opt);
	void AddOptions( list<Option> l);

	bool Parse(int argc, char ** const argv);

	bool HasReminder();
	list<string> GetReminder();

	string GetHelptext();

	virtual ~ArgParser();

private:
	int FindIndex(char c);

	vector<Option> opts;
	string shortargs;
	vector<struct option> longopts;
	list<string> reminder;
};

} // NS Utils
#endif // ARGPARSER_H
