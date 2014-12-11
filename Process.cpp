#include <sstream>

#include "Process.h"


tuple<bool, string> Utils::Process::Exec(const string &cmd)
{
	FILE* pipe = popen(cmd.c_str(), "re");

	if (!pipe)
	{
		return make_tuple(false,"Failed popen");
	}

	char buffer[128];
	stringstream result;

	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != NULL)
		{
			result << buffer;
		}
	}
	int res = pclose(pipe);

	bool ret = !((res < 0 ) ||  WEXITSTATUS(res) != 0);

	return make_tuple( ret, result.str());
}
