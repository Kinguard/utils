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
