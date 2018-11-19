cmake_minimum_required(VERSION 2.6)
project( utils )

set (UTILS_VERSION_MAJOR 1)
set (UTILS_VERSION_MINOR 5)
set (UTILS_VERSION_PATCH 12)
set (UTILS_VERSION "${UTILS_VERSION_MAJOR}.${UTILS_VERSION_MINOR}")
set (UTILS_VERSION_FULL "${UTILS_VERSION}.${UTILS_VERSION_PATCH}")
set (UTILS_APP_NAME "libutils")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tor Krill <tor@openproducts.se>") #required
SET(CPACK_PACKAGE_VERSION_MAJOR ${UTILS_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${UTILS_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${UTILS_VERSION_PATCH})

INCLUDE(CPack)


configure_file (
	"${PROJECT_SOURCE_DIR}/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h"
)

configure_file (
	"${PROJECT_SOURCE_DIR}/libutils.pc.in"
	"${PROJECT_BINARY_DIR}/libutils.pc"
	@ONLY
)

set( utils_headers
	ArgParser.h
	Application.h
	CircularBuffer.h
	ConfigFile.h
	ClassTools.h
	Condition.h
	Exceptions.h
	FileUtils.h
	Logger.h
	Mutex.h
	NetUtils.h
	NetServer.h
	Process.h
	Regex.h
	Shm.h
	SimpleHttpClient.h
	Socket.h
	StateMachine.h
	String.h
	Thread.h
	Timer.h
	UserGroups.h
	)

set( utils_src
	ArgParser.cpp
	Application.cpp
	CircularBuffer.cpp
	ConfigFile.cpp
	Condition.cpp
	FileUtils.cpp
	Logger.cpp
	Mutex.cpp
	NetUtils.cpp
	NetServer.cpp
	Process.cpp
	Regex.cpp
	Shm.cpp
	SimpleHttpClient.cpp
	Socket.cpp
	StateMachine.cpp
	String.cpp
	Thread.cpp
	Timer.cpp
	UserGroups.cpp
	)

add_definitions( -std=c++0x )

# add_library( util_objs OBJECT ${utils_src} )
add_library( utils_static STATIC ${utils_src} )
add_library( utils SHARED ${utils_src} )

target_link_libraries( utils -pthread -lrt)

set_target_properties( utils PROPERTIES
		VERSION ${UTILS_VERSION_FULL}
		SOVERSION ${UTILS_VERSION_MAJOR}
	)

set_target_properties( utils_static PROPERTIES
		OUTPUT_NAME utils
	)

add_subdirectory( tests )

enable_testing()
add_test( NAME TestUtils COMMAND testutils )

install( FILES ${utils_headers} DESTINATION include/libutils )
install(FILES "${PROJECT_BINARY_DIR}/libutils.pc" DESTINATION lib/pkgconfig)
install(FILES "${PROJECT_BINARY_DIR}/libutils.pc" DESTINATION share/pkgconfig)
install( TARGETS utils utils_static DESTINATION lib )
