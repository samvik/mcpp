cmake_minimum_required(VERSION 3.0.0)
project(mcpp) 

set(MCPP_VERSION 0.1.0)
set(MCPP_ABI_VERSION 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

file(GLOB SRC_LIST mcpp/*.cpp mcpp/*.h)

add_library(mcpp SHARED ${SRC_LIST})
set_target_properties (mcpp PROPERTIES
	OUTPUT_NAME mcpp
	VERSION ${MCPP_VERSION}
	SOVERSION ${MCPP_ABI_VERSION}
)

add_library(mcpp_static STATIC ${SRC_LIST})
set_target_properties(mcpp_static PROPERTIES
	OUTPUT_NAME mcpp
)

find_package(PkgConfig)

pkg_check_modules(ZMQ libzmq REQUIRED)
target_link_libraries(mcpp ${ZMQ_LIBRARIES})
target_link_libraries(mcpp_static ${ZMQ_LIBRARIES})
include_directories(${ZMQ_INCLUDE_DIRS})

pkg_check_modules(JSONCPP jsoncpp REQUIRED)
target_link_libraries(mcpp ${JSONCPP_LIBRARIES})
target_link_libraries(mcpp_static ${JSONCPP_LIBRARIES})
include_directories(${JSONCPP_INCLUDE_DIRS})

# Generate pc file
set(PKG_CONFIG_REQUIRES jsoncpp libzmq)
set(PKG_CONFIG_LIBDIR "\${prefix}/lib")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/include")
set(PKG_CONFIG_LIBS "-L\${libdir} -lmcpp")
set(PKG_CONFIG_CFLAGS "-I\${includedir} -std=c++11")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/mcpp.pc.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/mcpp.pc"
)

# Install
install(TARGETS mcpp LIBRARY
				DESTINATION lib
				COMPONENT runtime)

install(TARGETS mcpp_static ARCHIVE
				DESTINATION lib
				COMPONENT runtime)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mcpp/
				DESTINATION "include/mcpp"
				COMPONENT development
				FILES_MATCHING PATTERN "*.h")

install(FILES "${CMAKE_BINARY_DIR}/mcpp.pc"
				DESTINATION lib/pkgconfig
				COMPONENT development)
