cmake_minimum_required(VERSION 3.10)

project(PhyDB)

set(LEF_INCLUDE /net/ohm/export/cdgc/michael/EDA/lef/lef)
set(DEF_INCLUDE /net/ohm/export/cdgc/michael/EDA/def/def)

include_directories(${DEF_INCLUDE} ${LEF_INCLUDE})
include_directories(include)

link_directories(/usr/lib64)

FILE(GLOB SOURCES "src/*.cpp")
add_subdirectory(test)
add_subdirectory(src)
add_executable(PhyDB_test ${SOURCES})

target_link_libraries(PhyDB_test -Wno-unused-variable
/net/ohm/export/cdgc/michael/EDA/def/lib/libdef.a
	    /net/ohm/export/cdgc/michael/EDA/lef/lib/liblef.a
	    /net/ohm/export/cdgc/michael/boost-1.61.0/lib/libboost_serialization.a
 )
