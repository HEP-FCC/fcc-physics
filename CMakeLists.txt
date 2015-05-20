cmake_minimum_required(VERSION 2.8)
project(analysiscpp CXX)

# Set up C++11
set(CPP11FLAGS "-std=c++11")
if (${APPLE})
    set(CPP11FLAGS "-std=c++11\ -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPP11FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")

# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{ALBERS} $ENV{FCCEDM})

find_package(alberscore REQUIRED)
message(STATUS "Found albers-core: ${alberscore_DIR}")
# Make sure the library is found.
# Not the case if LD_LIBRARY_PATH is wrong
find_library(ALBERSLIB albers)
if (NOT ALBERSLIB)
    message(FATAL_ERROR "libalbers.so(dylib) cannot be found dynamically. Make sure you have sourced albers-core init*.sh file to set up your environment to use albers")
endif()

find_package(fccedm REQUIRED)
message(STATUS "Found fcc-edm: ${fccedm_DIR}")
# find_library(FCCEDMLIB datamodel HINTS ${fccedm_LIBRARIES})
# message("FCCEDM ${FCCEDMLIB}")

find_package(ROOT REQUIRED)
find_package(Fastjet)

# set up include-directories
include_directories(
  "${ROOT_INCLUDE_DIR}"
  "${ALBERSCORE_INCLUDE_DIRS}"
  "${FCCEDM_INCLUDE_DIRS}"
  "${fastjet_INCLUDE_DIR}"
  )  

# Add sub-directories
add_subdirectory(example)
add_subdirectory(example-lib)
add_subdirectory(tools)

