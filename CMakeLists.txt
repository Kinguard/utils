cmake_minimum_required(VERSION 2.6)
project( utils )

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Tor Krill <tor@openproducts.se>") #required
 
INCLUDE(CPack)

set (UTILS_VERSION_MAJOR 1)
set (UTILS_VERSION_MINOR 0)
set (UTILS_VERSION_PATCH 0)
set (UTILS_VERSION "${UTILS_VERSION_MAJOR}.${UTILS_VERSION_MINOR}")
set (UTILS_VERSION_FULL "${UTILS_VERSION}.${UTILS_VERSION_PATCH}")
set (UTILS_APP_NAME "libutils")

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
	CircularBuffer.h
	Condition.h
	FileUtils.h
	Logger.h
	Mutex.h
	NetUtils.h
	Shm.h
	SimpleHttpClient.h
	Socket.h
	String.h
	Thread.h
	)

set( utils_src
	CircularBuffer.cpp
	Condition.cpp
	FileUtils.cpp
	Logger.cpp
	Mutex.cpp
	NetUtils.cpp
	Shm.cpp
	SimpleHttpClient.cpp
	Socket.cpp
	String.cpp
	Thread.cpp
	)

add_definitions( -std=c++0x )

add_library( utils SHARED ${utils_src} )

target_link_libraries( utils -pthread -lrt)

set_target_properties( utils PROPERTIES
		VERSION ${UTILS_VERSION_FULL}
		SOVERSION ${UTILS_VERSION_MAJOR}
	)

add_subdirectory( tests )

enable_testing()
add_test( NAME TestUtils COMMAND testutils )

install( FILES ${utils_headers} DESTINATION include/libutils )
install(FILES "${PROJECT_BINARY_DIR}/libutils.pc" DESTINATION lib/pkgconfig)
install( TARGETS utils DESTINATION lib )
