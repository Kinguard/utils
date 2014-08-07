#ifndef REGEX_H
#define REGEX_H

#include <regex.h>
#include <sys/types.h>

#include <string>
#include <vector>

using namespace std;

namespace Utils {

class Regex
{
public:

	typedef regmatch_t Match;

	Regex();
	Regex( const string& pattern, bool insensitive = false);

	vector<Match> DoMatch(const string& line, int maxmatch = 10);

	void Compile( const string& pattern, bool insensitive = false );

	virtual ~Regex();
private:

	void Free();

	bool valid;
	regex_t preg;
};

} /* namespace Utils */
#endif // REGEX_H
