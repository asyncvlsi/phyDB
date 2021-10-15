cmake_minimum_required(VERSION 3.10)

project(PhyDB
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(include)

############################################################################
# Detect if the environment variable ACT_HOME can be found
############################################################################
message(STATUS "Detecting environment variable ACT_HOME...")
if (DEFINED ENV{ACT_HOME})
    message(STATUS "Environment variable ACT_HOME detected: " $ENV{ACT_HOME})
else ()
    message(FATAL_ERROR "Environment variable ACT_HOME is not found")
endif ()
include_directories($ENV{ACT_HOME}/include)
link_directories($ENV{ACT_HOME}/lib)

############################################################################
# Detect the installation path of LEF parser and DEF parser,
# and check if the liblef and libdef library can be found or not
############################################################################
message(STATUS "Detecting environment variable LEF_ROOT...")
set(LEF_ROOT "")
if (DEFINED ENV{LEF_ROOT})
    message(STATUS "Environment variable LEF_ROOT detected: " $ENV{LEF_ROOT})
    include_directories($ENV{LEF_ROOT}/include)
    link_directories($ENV{LEF_ROOT}/lib)
    set(LEF_ROOT $ENV{LEF_ROOT})
else ()
    message(STATUS "Environment variable LEF_ROOT not found, using ACT_HOME instead")
    include_directories($ENV{ACT_HOME}/include)
    link_directories($ENV{ACT_HOME}/lib)
    set(LEF_ROOT $ENV{ACT_HOME})
endif ()
find_library(
    LEF_LIBRARY
    NAMES lef
    PATHS ${LEF_ROOT}/lib
    REQUIRED
)
if (NOT LEF_LIBRARY)
    message(FATAL_ERROR "Cannot find liblef.a")
else ()
    message(STATUS "Found liblef.a: " ${LEF_LIBRARY})
endif ()

message(STATUS "Detecting environment variable DEF_ROOT...")
set(DEF_ROOT "")
if (DEFINED ENV{DEF_ROOT})
    message(STATUS "Environment variable DEF_ROOT detected: " $ENV{DEF_ROOT})
    include_directories($ENV{DEF_ROOT}/include)
    link_directories($ENV{DEF_ROOT}/lib)
    set(DEF_ROOT $ENV{DEF_ROOT})
else ()
    message(STATUS "Environment variable DEF_ROOT not found, using ACT_HOME instead")
    include_directories($ENV{ACT_HOME}/include)
    link_directories($ENV{ACT_HOME}/lib)
    set(DEF_ROOT $ENV{ACT_HOME})
endif ()
find_library(
    DEF_LIBRARY
    NAMES def
    PATHS ${DEF_ROOT}/lib
    REQUIRED
)
if (NOT DEF_LIBRARY)
    message(FATAL_ERROR "Cannot find libdef.a")
else ()
    message(STATUS "Found libdef.a: " ${DEF_LIBRARY})
endif ()

############################################################################
# Check if the galois_eda library is installed
############################################################################
message(STATUS "Detecting Galois libraries...")
set(Galois_FOUND TRUE)
find_library(
    GALOIS_EDA_LIBRARY
    NAMES galois_eda
    PATHS $ENV{ACT_HOME}/lib
)
if (GALOIS_EDA_LIBRARY)
    set(Galois_LIBRARIES ${GALOIS_EDA_LIBRARY})
    message(STATUS "Found libgalois_eda.a: " ${GALOIS_EDA_LIBRARY})
else()
    message(STATUS "Cannot find libgalois_eda.a")
    set(Galois_FOUND FALSE)
endif()
find_library(
    GALOIS_SHMEM_LIBRARY
    NAMES galois_shmem
    PATHS $ENV{ACT_HOME}/lib
)
if (GALOIS_SHMEM_LIBRARY)
    set(Galois_LIBRARIES ${Galois_LIBRARIES} ${GALOIS_SHMEM_LIBRARY})
    message(STATUS "Found libgalois_shmem.a: " ${GALOIS_SHMEM_LIBRARY})
else()
    message(STATUS "Cannot find libgalois_shmem.a")
    set(Galois_FOUND FALSE)
endif()
if (UNIX AND (NOT APPLE))
    find_library(
        NUMA_LIBRARY
        NAMES numa
    )
    if (NUMA_LIBRARY)
        set(Galois_LIBRARIES ${Galois_LIBRARIES} ${NUMA_LIBRARY})
        message(STATUS "Found libnuma: " ${NUMA_LIBRARY})
    else()
        message(STATUS "Cannot find libnuma")
        set(Galois_FOUND FALSE)
    endif()
endif()
if (Galois_FOUND)
    set(PHYDB_USE_GALOIS 1)
else ()
    set(PHYDB_USE_GALOIS 0)
endif ()
configure_file(${INC_DIR}/config.h.in ${INC_DIR}/config.h)

############################################################################
# Check Boost library
############################################################################
message(STATUS "Detecting Boost libraries...")
#set(Boost_USE_STATIC_LIBS ON) # turn on/off static linking
find_package(Boost 1.71.0 COMPONENTS log_setup log filesystem REQUIRED)
message(STATUS "Boost include path: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost library path: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost libs: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})

# Set a default build type if none was specified
set(default_build_type "RELEASE")
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "DEBUG" "RELEASE")
endif ()

# Set compilation flags
add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Werror=return-type -pedantic)
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fPIC")

# Set the output directory of static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Set the output directory of executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

FILE(GLOB SOURCES "src/*.cpp")
add_subdirectory(test)

# create the core library--phydb
add_library(
    phydb
    STATIC
    ${SOURCES}
)
target_link_libraries(
    phydb
    lef def
    ${Boost_LIBRARIES}
    ${Galois_LIBRARIES}
)

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
