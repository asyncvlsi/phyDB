############################################################################
# Check if the galois_eda library is installed
#
# This cmake file will define the following variables
#    Galois_FOUND, whether the galois core library and galois_eda library can be found
#    Galois_LIBRARIES, the list of galois libraries
############################################################################
message(STATUS "Detecting Galois libraries...")
set(Galois_FOUND TRUE)

# Find galois_eda library
find_library(
    GALOIS_EDA_LIBRARY
    NAMES galois_eda
    PATHS $ENV{ACT_HOME}/lib
)
if(GALOIS_EDA_LIBRARY)
    set(Galois_LIBRARIES ${GALOIS_EDA_LIBRARY})
    message(STATUS "Found libgalois_eda.a: " ${GALOIS_EDA_LIBRARY})
else()
    message(STATUS "Cannot find libgalois_eda.a")
    set(Galois_FOUND FALSE)
endif()

# Find galois_shmem library
find_library(
    GALOIS_SHMEM_LIBRARY
    NAMES galois_shmem
    PATHS $ENV{ACT_HOME}/lib
)
if(GALOIS_SHMEM_LIBRARY)
    set(Galois_LIBRARIES ${Galois_LIBRARIES} ${GALOIS_SHMEM_LIBRARY})
    message(STATUS "Found libgalois_shmem.a: " ${GALOIS_SHMEM_LIBRARY})
else()
    message(STATUS "Cannot find libgalois_shmem.a")
    set(Galois_FOUND FALSE)
endif()

# Find numa for linux
if(UNIX AND (NOT APPLE))
    find_library(
        NUMA_LIBRARY
        NAMES numa
    )
    if(NUMA_LIBRARY)
        set(Galois_LIBRARIES ${Galois_LIBRARIES} ${NUMA_LIBRARY})
        message(STATUS "Found libnuma: " ${NUMA_LIBRARY})
    else()
        message(STATUS "Cannot find libnuma")
        set(Galois_FOUND FALSE)
    endif()
endif()

# Configure the config.h header file
if(Galois_FOUND)
    set(PHYDB_USE_GALOIS 1)
else()
    set(PHYDB_USE_GALOIS 0)
endif()
configure_file(
    phydb/timing/config.h.in
    phydb/timing/config.h
)
