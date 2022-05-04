cmake_minimum_required(VERSION 3.10)

project(
    PhyDB
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add sub-cmake files to the search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

############################################################################
# Detect if the environment variable ACT_HOME can be found
############################################################################
message(STATUS "Detecting environment variable ACT_HOME...")
if(DEFINED ENV{ACT_HOME})
    message(STATUS "Environment variable ACT_HOME detected: " $ENV{ACT_HOME})
else()
    message(FATAL_ERROR "Environment variable ACT_HOME is not found")
endif()
include_directories($ENV{ACT_HOME}/include)
link_directories($ENV{ACT_HOME}/lib)
find_library(LEF_LIBRARY NAMES lef PATHS $ENV{ACT_HOME}/lib REQUIRED)
message(STATUS "Found liblef.a: " ${LEF_LIBRARY})
find_library(DEF_LIBRARY NAMES def PATHS $ENV{ACT_HOME}/lib REQUIRED)
message(STATUS "Found libdef.a: " ${DEF_LIBRARY})

# Find galois_eda
include(cmake/FindGaloisEDA.cmake)

############################################################################
# Check Boost library
############################################################################
message(STATUS "Detecting Boost libraries...")
#set(Boost_USE_STATIC_LIBS ON) # turn on/off static linking
find_package(Boost 1.60.0 COMPONENTS log_setup log filesystem REQUIRED)
message(STATUS "Boost include path: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost library path: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})

# Set a default build type if none was specified
set(default_build_type "RELEASE")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "DEBUG" "RELEASE")
endif()

# Set compilation flags
add_compile_options(
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Werror=return-type
    -pedantic
)
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fPIC")

# Set the output directory of static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Set the output directory of executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(
    GLOB_RECURSE SOURCES
    phydb/*.cpp
)
add_subdirectory(test)

# Create the core library--phydb
add_library(
    phydb
    STATIC
    ${SOURCES}
)
target_link_libraries(
    phydb
    ${LEF_LIBRARY} ${DEF_LIBRARY}
    ${Boost_LIBRARIES}
    ${Galois_LIBRARIES}
)

add_executable(PhyDB_test test/test.cpp)
target_link_libraries(PhyDB_test PRIVATE phydb)

add_executable(parser_test test/test_parser.cpp)
target_link_libraries(parser_test PRIVATE phydb)

############################################################################
# Specify the installation directory: ${ACT_HOME}
############################################################################
message(STATUS "Changing Installation directory to $ACT_HOME")
set(CMAKE_INSTALL_PREFIX $ENV{ACT_HOME} CACHE PATH "installation path" FORCE)
message(STATUS "Current installation directory: " ${CMAKE_INSTALL_PREFIX})

############################################################################
# Install header files
############################################################################
install(
    DIRECTORY phydb/
    DESTINATION include/phydb
    FILES_MATCHING
    PATTERN "*.h"
)

############################################################################
# Install library
############################################################################
install(
    TARGETS phydb
    DESTINATION lib
)
