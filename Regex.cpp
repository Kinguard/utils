#include <stdexcept>
#include <algorithm>

#include "Regex.h"

namespace Utils {

Regex::Regex() : valid(false)
{
}

Regex::Regex(const string &pattern, bool insensitive): Regex()
{
	this->Compile( pattern, insensitive );
}

vector<Regex::Match> Regex::DoMatch(const string &line, int maxmatch)
{

	if( ! this->valid )
	{
		throw runtime_error("No valid pattern compiled");
	}

	vector<Match> ret(maxmatch+1);
	//ret.reserve(maxmatch+1);

	int res = regexec( & this->preg, line.c_str(), maxmatch, &ret[0], 0);

	if( res != REG_NOERROR && res != REG_NOMATCH )
	{
		throw runtime_error("Error doing pattern match");
	}


	if( res == REG_NOMATCH )
	{
		//printf("No match found\n");
		return {};
	}

	auto pend = remove_if( ret.begin(),ret.end(),
						   [](Regex::Match m)
							{
								return ( m.rm_so < 1 )  && ( m.rm_eo < 1);
							});
	ret.erase(pend, ret.end());

	return ret;
}

void Regex::Compile(const string &pattern, bool insensitive)
{
	if( this->valid )
	{
		this->Free();
	}
	int flags = REG_EXTENDED;

	if( insensitive )
	{
		flags |= REG_ICASE;
	}

	if( regcomp( &this->preg, pattern.c_str(), flags ) != 0 )
	{
		this->valid = false;
		throw runtime_error("Failed to compile pattern");
	}

	this->valid = true;
}

Regex::~Regex()
{
	if( this->valid )
	{
		this->Free();
	}
}

void Regex::Free()
{
	regfree( &this->preg );
	this->valid = false;
}

} // Namespace Utils
