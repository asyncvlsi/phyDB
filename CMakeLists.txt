cmake_minimum_required(VERSION 3.16)

project(PhyDB
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

############################################################################
# Detect the installation path of LEF parser and DEF parser,
# and check if the liblef and libdef library can be found or not
############################################################################
message(STATUS "Detecting environment variable LEF_ROOT...")
if (DEFINED ENV{LEF_ROOT})
    message(STATUS "Environment variable LEF_ROOT detected: " $ENV{LEF_ROOT})
else ()
    message(FATAL_ERROR "Environment variable LEF_ROOT not found")
endif ()
include_directories($ENV{LEF_ROOT}/include)
link_directories($ENV{LEF_ROOT}/lib)
find_library(
    LEF_LIBRARY
    NAMES lef
    PATHS $ENV{LEF_ROOT}/lib
    REQUIRED
)
if (NOT LEF_LIBRARY)
    message(FATAL_ERROR "Cannot find liblef.a")
else ()
    message(STATUS "Found liblef.a")
endif ()

message(STATUS "Detecting environment variable DEF_ROOT...")
if (DEFINED ENV{DEF_ROOT})
    message(STATUS "Environment variable LEF_ROOT detected: " $ENV{DEF_ROOT})
else ()
    message(FATAL_ERROR "Environment variable DEF_ROOT not found")
endif ()
include_directories($ENV{DEF_ROOT}/include)
link_directories($ENV{DEF_ROOT}/lib)
find_library(
    DEF_LIBRARY
    NAMES def
    PATHS $ENV{DEF_ROOT}/lib
    REQUIRED
)
if (NOT DEF_LIBRARY)
    message(FATAL_ERROR "Cannot find libdef.a")
else ()
    message(STATUS "Found libdef.a")
endif ()


############################################################################
# Detect if the environment variable ACT_HOME can be found
############################################################################
message(STATUS "Detecting environment variable ACT_HOME...")
if (DEFINED ENV{ACT_HOME})
    message(STATUS "Environment variable LEF_ROOT detected: " $ENV{ACT_HOME})
else ()
    message(FATAL_ERROR "Environment variable ACT_HOME is not found")
endif ()
include_directories($ENV{ACT_HOME}/include)
link_directories($ENV{ACT_HOME}/lib)

############################################################################
# Check if the galois_eda library is installed
############################################################################
find_library(
    GALOIS_EDA
    NAMES galois_eda
    PATHS $ENV{ACT_HOME}/lib
)
if (NOT GALOIS_EDA)
    set(PHYDB_USE_GALOIS 0)
    message(STATUS "Cannot find libgalois_eda.a")
else ()
    set(PHYDB_USE_GALOIS 1)
    message(STATUS "Found libgalois_eda.a")
endif ()
configure_file(${INC_DIR}/config.h.in ${INC_DIR}/config.h)

############################################################################
# Check Boost library
############################################################################
find_package(Boost 1.71.0)
message(STATUS "Boost library path: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost include path: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})

############################################################################
# Add a flex dependency
############################################################################
find_package(FLEX 2.5.0 REQUIRED)
add_custom_command(
    OUTPUT ${SRC_DIR}/scanner.cpp
    COMMAND ${FLEX_EXECUTABLE} -o ${SRC_DIR}/scanner.cpp ${INC_DIR}/scanner.l
    COMMAND sed 's/register//g' ${SRC_DIR}/scanner.cpp > ${SRC_DIR}/scanner.cppx # if flex 2.6.4 is used, then this command can be removed
    COMMAND mv -f ${SRC_DIR}/scanner.cppx ${SRC_DIR}/scanner.cpp # if flex 2.6.4 is used, then this command can be removed
    DEPENDS ${INC_DIR}/scanner.l
)
add_custom_target(flex_target ALL DEPENDS ${SRC_DIR}/scanner.cpp)

############################################################################
# Add a bison dependency
############################################################################
find_package(BISON 3.2.0 REQUIRED)
add_custom_command(
    OUTPUT ${SRC_DIR}/parser.cpp
    COMMAND ${BISON_EXECUTABLE} -o ${SRC_DIR}/parser.cpp ${INC_DIR}/parser.y
    COMMAND mv ${SRC_DIR}/parser.hpp ${INC_DIR}/parser.hpp
    DEPENDS ${INC_DIR}/parser.y
)
add_custom_target(bison_target ALL DEPENDS ${SRC_DIR}/parser.cpp)

# Set a default build type if none was specified
set(default_build_type "RELEASE")
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "DEBUG" "RELEASE")
endif ()

# Set compilation flags
add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fPIC")

# Set the output directory of static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Set the output directory of executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(include)
FILE(GLOB SOURCES "src/*.cpp")
add_subdirectory(test)

# create the core library--phydb
add_library(
    phydb
    STATIC
    ${SOURCES}
    ${SRC_DIR}/scanner.cpp
    ${SRC_DIR}/parser.cpp
)
add_dependencies(phydb flex_target bison_target)
target_link_libraries(phydb lef def)

add_executable(PhyDB_test test/test.cpp)
target_link_libraries(PhyDB_test PRIVATE phydb)

add_executable(parser_test test/test_parser.cpp)
target_link_libraries(parser_test PRIVATE phydb)

############################################################################
# specify the installation directory: ${ACT_HOME}
############################################################################
message(STATUS "Changing Installation directory to $ACT_HOME")
set(CMAKE_INSTALL_PREFIX $ENV{ACT_HOME} CACHE PATH "installation path" FORCE)
message(STATUS "Current installation directory: " ${CMAKE_INSTALL_PREFIX})


############################################################################
# Install header files
############################################################################
install(
    DIRECTORY include/
    DESTINATION include/phydb
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

############################################################################
# Install library
############################################################################
install(
    TARGETS phydb
    DESTINATION lib
)
