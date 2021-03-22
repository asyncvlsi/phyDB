cmake_minimum_required(VERSION 3.16)

project(PhyDB
		LANGUAGES CXX)

######################################
# Detect if Boost library exists
######################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.61.0 COMPONENTS serialization REQUIRED)
message(STATUS "Boost library path: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost include path: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

######################################
# detect the installation path of LEF parser and DEF parser,
# and check if the liblef and libdef library can be found or not
######################################
message(STATUS "Detecting environment variable LEF_ROOT...")
if (DEFINED ENV{LEF_ROOT})
	message(STATUS "Environment variable LEF_ROOT detected: " $ENV{LEF_ROOT})
else ()
	message(FATAL_ERROR "Environment variable LEF_ROOT not found")
endif ()
include_directories($ENV{LEF_ROOT}/include)
link_directories($ENV{LEF_ROOT}/lib)
find_library(LEF_LIBRARY
		NAMES lef
		PATHS $ENV{LEF_ROOT}/lib
		REQUIRED)
if (NOT LEF_LIBRARY)
	message(FATAL_ERROR "liblef.a is not found")
else ()
	message(STATUS "liblef.a found")
endif()
link_libraries(liblef.a)

message(STATUS "Detecting environment variable DEF_ROOT...")
if (DEFINED ENV{DEF_ROOT})
	message(STATUS "Environment variable LEF_ROOT detected: " $ENV{DEF_ROOT})
else ()
	message(FATAL_ERROR "Environment variable DEF_ROOT not found")
endif ()
include_directories($ENV{DEF_ROOT}/include)
link_directories($ENV{DEF_ROOT}/lib)
find_library(DEF_LIBRARY
		NAMES def
		PATHS $ENV{DEF_ROOT}/lib
		REQUIRED)
if (NOT DEF_LIBRARY)
	message(FATAL_ERROR "libdef.a is not found")
else ()
	message(STATUS "libdef.a found")
endif()
link_libraries(libdef.a)

set(CFLAGS "${CFLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 ${EXTRA_FLAGS} -fopenmp ${CFLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -std=c++17 -O3 ${EXTRA_FLAGS} -fopenmp ${CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -O3 ${EXTRA_FLAGS} -fopenmp ${CFLAGS}")

# Set the output directory of static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Set the output directory of executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

link_directories(/usr/lib64)

include_directories(include)
FILE(GLOB SOURCES "src/*.cpp")
add_subdirectory(test)
add_subdirectory(src)

add_executable(PhyDB_test
		test/test.cpp)
target_link_libraries(PhyDB_test
		PRIVATE phydb
		)

######################################
# Change FALSE to TRUE if you want to change the installation directory to ${ACT_HOME}
######################################

message(STATUS "Detecting environment variable ACT_HOME...")
if (DEFINED ENV{ACT_HOME})
	message(STATUS "Environment variable ACT_HOME detected: " $ENV{ACT_HOME})
else ()
	message(FATAL_ERROR "Environment variable ACT_HOME not found")
endif ()
message(STATUS "Changing Installation directory to ACT_HOME")
set(CMAKE_INSTALL_PREFIX $ENV{ACT_HOME} CACHE PATH "installation path" FORCE)
message(STATUS "Current installation directory: " ${CMAKE_INSTALL_PREFIX})


######################################
# Install header files
######################################
install(DIRECTORY include/ DESTINATION include/phydb COMPONENT Development FILES_MATCHING PATTERN "*.h")

######################################
# Install library
######################################
install(TARGETS phydb
		DESTINATION lib)

