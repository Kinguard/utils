#include "TestUserGroups.h"
#include "UserGroups.h"
#include "Exceptions.h"

#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION ( TestUserGroups );

using namespace Utils;
using namespace std;

void TestUserGroups::setUp()
{
}

void TestUserGroups::tearDown()
{
}

void TestUserGroups::TestUser()
{
	CPPUNIT_ASSERT_EQUAL( (uid_t)0, User::UserToUID("root") );
	CPPUNIT_ASSERT_EQUAL( std::string("root"), User::UIDToUser(0) );
	CPPUNIT_ASSERT_THROW( User::UserToUID("unknown user"), ErrnoException );
	// Hopefully we will never have user 999
	CPPUNIT_ASSERT_THROW( User::UIDToUser(999), ErrnoException );

	CPPUNIT_ASSERT_THROW( User::Groups("unknown user"), ErrnoException );

	list<pair<string,gid_t> > glist = User::Groups("root");
	CPPUNIT_ASSERT_EQUAL( (long unsigned int)1, glist.size() );
	CPPUNIT_ASSERT_EQUAL( std::string("root"), glist.front().first );
	CPPUNIT_ASSERT_EQUAL( (unsigned int)0, glist.front().second );

}

void
TestUserGroups::TestGroup ()
{
	CPPUNIT_ASSERT_EQUAL( (uid_t)0, Group::GroupToGID("root") );
	CPPUNIT_ASSERT_EQUAL( std::string("root"), Group::GIDToGroup(0) );

	CPPUNIT_ASSERT_THROW( Group::GroupToGID("unknown group"), ErrnoException );
	CPPUNIT_ASSERT_THROW( Group::GIDToGroup(999), ErrnoException );
}
