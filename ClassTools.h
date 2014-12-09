/*
 * ClassTools.h
 *
 *  Created on: Dec 27, 2012
 *      Author: tor
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
