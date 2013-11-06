/*
 * UserGroups.h
 *
 *  Created on: Nov 6, 2013
 *      Author: tor
 */

#ifndef USERGROUPS_H_
#define USERGROUPS_H_

#include <grp.h>

#include <list>
#include <utility>
#include <string>


namespace Utils
{

namespace User
{
	uid_t UserToUID(const std::string& username);
	std::string UIDToUser(uid_t uid);
	std::list<std::pair<std::string,gid_t> > Groups(const std::string& user);
}

namespace Group
{
	std::string GIDToGroup(gid_t gid);
	gid_t GroupToGID(const std::string& groupname);
}

}



#endif /* USERGROUPS_H_ */
