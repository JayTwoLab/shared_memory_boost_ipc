cmake_minimum_required(VERSION 3.16)
project(shared_memory_boost_ipc LANGUAGES CXX)

if(NOT UNIX OR APPLE)
  message(FATAL_ERROR "This project supports Linux only.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS)

add_subdirectory(writer)
add_subdirectory(reader)

###################################
# Building commands (Case Rocky Linux)
###################################
# rm -rf build
#
# cmake -S . -B build -DBOOST_ROOT=/usr -DBOOST_INCLUDEDIR=/usr/include -DBOOST_LIBRARYDIR=/usr/lib64   -DBoost_NO_SYSTEM_PATHS=ON -DBoost_DEBUG=ON
#
# cmake --build build
#

####################################
# NOTICE:
####################################
# Current shared memory is not erased automatically.
#  For reader/writer running sequence.
# If you remove shared memory, run writer with --remove option.
#

