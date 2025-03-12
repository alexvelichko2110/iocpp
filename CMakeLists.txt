cmake_minimum_required (VERSION 3.22)

set (PROJECT iocpp)

# set project name
project (${PROJECT})

# include necessary submodules
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

#set the default path for output
#set(EXECUTABLE_OUTPUT_PATH ${radar_SOURCE_DIR}/build)
#set(LIBRARY_OUTPUT_PATH ${radar_SOURCE_DIR}/build)

# pkg-config
include (FindPkgConfig)


add_definitions(-c -pipe -O0 -g -Wall)

# set search include path
include_directories(".")


# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# build

add_definitions(-std=c++11)

set(HEADERS
    ./buffer.h
    ./socket.h
    ./wait.h
    ./timer.h
)

set (SOURCES
    ./buffer.cpp
    ./socket.cpp
    ./wait.cpp
    ./timer.cpp
)

add_library(${PROJECT} SHARED ${SOURCES})

add_custom_target(clean-all
    COMMAND rm -f Makefile
    COMMAND rm -f cmake_install.cmake
    COMMAND rm -f install_manifest.txt
    COMMAND rm -f CMakeCache.txt
    COMMAND rm -R -f CMakeFiles
)

add_custom_target(install-all
    COMMAND sudo mkdir -p /usr/include/iocpp/
    COMMAND cp ${HEADERS} /usr/include/iocpp/  
    COMMAND cp ./lib${PROJECT}.so /usr/lib/
)
