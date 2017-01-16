#include "TestConfigFile.h"

CPPUNIT_TEST_SUITE_REGISTRATION ( TestConfigFile );

#include "Config.h"
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>

using namespace std;

#include "ConfigFile.h"


#if 0 // ????
int cp(const char *from, const char *to)
{
	int fd_to, fd_from;
	char buf[4096];
	ssize_t nread;
	int saved_errno;

	fd_from = open(from, O_RDONLY);
	if (fd_from < 0)
		return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		goto out_error;

	while (nread = read(fd_from, buf, sizeof buf), nread > 0)
	{
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fd_to, out_ptr, nread);

			if (nwritten >= 0)
			{
				nread -= nwritten;
				out_ptr += nwritten;
			}
			else if (errno != EINTR)
			{
				goto out_error;
			}
		} while (nread > 0);
	}

	if (nread == 0)
	{
		if (close(fd_to) < 0)
		{
			fd_to = -1;
			goto out_error;
		}
		close(fd_from);

		/* Success! */
		return 0;
	}

  out_error:
	saved_errno = errno;

	close(fd_from);
	if (fd_to >= 0)
		close(fd_to);

	errno = saved_errno;
	return -1;
}
#endif


void TestConfigFile::setUp()
{
	ofstream ofs("test.cfg");

	ofs << "# A comment\n"
		<< "A line without value\n"
		<< "\n"
		<< "key 1 = val 1\n"
		<< "\n"
		<< "key1 = val1\n"
		<< "key2 = A longer value\n"
		<< "key3 = 12\n"
		<< "\n";
	ofs.close();


	ofstream ofs2("test2.cfg");

	ofs2 << "# A comment\n"
		<< "A line without value\n"
		<< "\n"
		<< "\t[s1]\n"
		<< "key 1 = val 1\n"
		<< "\n"
		<< "key1 = val1\n"
		<< "[ s2\t]\n"
		<< "key2 = A longer value\n"
		<< "key3 = 12\n"
		<< "\n";
	ofs2.close();


}

void TestConfigFile::tearDown()
{
	unlink("test.cfg");
	unlink("test2.cfg");

	unlink("testsave.cfg");
}

void TestConfigFile::Test()
{
	Utils::ConfigFile f("test.cfg");

	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("noval", "default"), string("default"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key1", "default"), string("val1"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key 1", "default"), string("val 1"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key2"), string("A longer value"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key3"), string("12"));

	CPPUNIT_ASSERT_EQUAL( f["key3"], string("12"));

	f["newkey"] = "A test value";
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("newkey"), string("A test value"));

	CPPUNIT_ASSERT_NO_THROW( f.Sync() );

	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("noval", "default"), string("default"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key1", "default"), string("val1"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key 1", "default"), string("val 1"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key2"), string("A longer value"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key3"), string("12"));
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("newkey"), string("A test value"));

	f["key1"] = "New val";
	CPPUNIT_ASSERT_EQUAL( f.ValueOrDefault("key1", "default"), string("New val"));

}

void TestConfigFile::TestIni()
{
	list<string> data=
	{
		"#Hello world!",
		"[section1]",
		"aa = bb"
	};

	Utils::IniFile ini(data);

	//ini.Dump();
	CPPUNIT_ASSERT_EQUAL( ini.Value("section1","aa",""), string("bb"));
	CPPUNIT_ASSERT_EQUAL( ini.Value("section1","aas","cc"), string("cc"));

}

void TestConfigFile::TestIniS3ql()
{
	list<string> data=
	{
		";Hello world!",
		"[section1]",
		"aa : bb",
		"[s2]",
		"	hello:world"
	};

	Utils::IniFile ini(data, ":",';');

	//ini.Dump();

	CPPUNIT_ASSERT_EQUAL( ini.Value("section1","aa"), string("bb"));

	ini.UseSection("s2");
	CPPUNIT_ASSERT_EQUAL( ini.ValueOrDefault("hello",""), string("world"));
}

void TestConfigFile::TestIniFile()
{
	Utils::IniFile i("test2.cfg");
	i.Dump();
	CPPUNIT_ASSERT_EQUAL( i.Value("s1","key 1"), string("val 1"));
	CPPUNIT_ASSERT_EQUAL( i.Value("s1","key1"), string("val1"));
	CPPUNIT_ASSERT_EQUAL( i.Value("s2","key2"), string("A longer value"));
	CPPUNIT_ASSERT_EQUAL( i.Value("s2","key3"), string("12"));
}

void TestConfigFile::TestIniSaveFile()
{

	{
		Utils::IniFile i("test2.cfg");
		CPPUNIT_ASSERT_NO_THROW( i.Save() );
	}

	{
		Utils::IniFile i("test2.cfg");
		i.Dump();
		CPPUNIT_ASSERT_EQUAL( i.Value("s1","key 1"), string("val 1"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s1","key1"), string("val1"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s2","key2"), string("A longer value"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s2","key3"), string("12"));

		CPPUNIT_ASSERT_NO_THROW( i.Save("testsave.cfg") );
	}

	{
		Utils::IniFile i("testsave.cfg");
		i.Dump();
		CPPUNIT_ASSERT_EQUAL( i.Value("s1","key 1"), string("val 1"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s1","key1"), string("val1"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s2","key2"), string("A longer value"));
		CPPUNIT_ASSERT_EQUAL( i.Value("s2","key3"), string("12"));
	}

}
