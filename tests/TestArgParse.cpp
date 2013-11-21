#include "TestArgParse.h"

#include <vector>

#include "ArgParser.h"

using namespace  std;

CPPUNIT_TEST_SUITE_REGISTRATION ( TestArgParse );

void TestArgParse::setUp()
{
}

void TestArgParse::tearDown()
{
}
 
void TestArgParse::Test()
{
	vector<const char*> args = { "myapp", "-a", "-b", "--clong=cnew", "-d", "dnew", "r1", "r2"};

	Utils::ArgParser arg;

	arg.AddOptions( {
						Utils::Option('a',"along",Utils::Option::ArgNone,		"0",		"Min hjälptext"),
						Utils::Option('b',"blong",Utils::Option::ArgOptional,	"bval",		"Min hjälptext"),
						Utils::Option('c',"clong",Utils::Option::ArgOptional,	"cval",		"Min hjälptext"),
						Utils::Option('d',"dlong",Utils::Option::ArgRequired,	"dval",		"Min hjälptext"),
						Utils::Option('e',"elong",Utils::Option::ArgRequired,	"eval",		"Min hjälptext"),
						Utils::Option('f',"flong",Utils::Option::ArgNone,		"0",		"Min hjälptext"),
				} );


	CPPUNIT_ASSERT( !arg.HasReminder() );
#if 0
	cout << "--------------------"<< endl;
	for( auto x: arg)
	{
		cout << "Key: "<<x.first << " value: "<<x.second<<endl;
	}
#endif

	CPPUNIT_ASSERT( arg.Parse(args.size(), (char**)&args[0] ) );

#if 0
	cout << "--------------------"<< endl;
	for( auto x: arg)
	{
		cout << "Key: "<<x.first << " value: "<<x.second<<endl;
	}
#endif

	CPPUNIT_ASSERT( arg["along"] == "1" );
	CPPUNIT_ASSERT( arg["flong"] == "0" );
	CPPUNIT_ASSERT( arg["blong"] == "1" );
	CPPUNIT_ASSERT( arg["clong"] == "cnew" );
	CPPUNIT_ASSERT( arg["dlong"] == "dnew" );
	CPPUNIT_ASSERT( arg["elong"] == "eval" );
	CPPUNIT_ASSERT( arg.HasReminder() );

	list<string> rem = arg.GetReminder();
	CPPUNIT_ASSERT( rem.size()== 2);
	CPPUNIT_ASSERT( rem.front() == "r1");
	CPPUNIT_ASSERT( rem.back() == "r2");

}
