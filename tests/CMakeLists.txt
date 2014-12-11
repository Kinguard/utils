include( FindPkgConfig )

pkg_check_modules( CPPUNIT cppunit>=1.12.1 )

set( TESTUTILS_PATH ${CMAKE_CURRENT_SOURCE_DIR})

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/Config.h"
)


set( testutils_src
	SocketTest.cpp
	StringTest.cpp
	TestArgParse.cpp
	TestCircularBuffer.cpp
	TestConfigFile.cpp
	test.cpp
	TestFileUtils.cpp
	TestLogger.cpp
	TestNetUtils.cpp
	TestNetServer.cpp
	TestProcess.cpp
	TestRegex.cpp
	TestShm.cpp
	TestSimpleHttpClient.cpp
	TestTimer.cpp
	TestUserGroups.cpp
	)
include_directories( "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

add_executable( testutils ${testutils_src} )

target_link_libraries( testutils utils ${CPPUNIT_LDFLAGS})
