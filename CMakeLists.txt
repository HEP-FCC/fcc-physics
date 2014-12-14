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
find_package(fccedm REQUIRED)
find_package(ROOT REQUIRED)

# set up include-directories
include_directories(
  "${ROOT_INCLUDE_DIR}"
  "${ALBERSCORE_INCLUDE_DIRS}"
  "${FCCEDM_INCLUDE_DIRS}"
  )  

# Add sub-directories
add_subdirectory(example)

