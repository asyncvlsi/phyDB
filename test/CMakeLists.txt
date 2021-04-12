cmake_minimum_required(VERSION 3.10)

set(LEF_INCLUDE /net/ohm/export/cdgc/michael/EDA/lef/lef)
set(DEF_INCLUDE /net/ohm/export/cdgc/michael/EDA/def/def)

include_directories(${DEF_INCLUDE} ${LEF_INCLUDE})
include_directories(../include)

link_directories(/usr/lib64)
link_directories(../src)

