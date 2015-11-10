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

#ifndef CLASSTOOLS_H_
#define CLASSTOOLS_H_

namespace Utils{

class NoCopy
{
public:
	NoCopy(const NoCopy&) = delete;
	NoCopy operator=(const NoCopy&) = delete;

	NoCopy()
	{
	}

	virtual ~NoCopy()
	{
	}

};


} // NS utils

#endif /* CLASSTOOLS_H_ */
