cmake_minimum_required(VERSION 2.8)
project(pythiafcc CXX)

set(pythia8 $ENV{PYTHIA8_DIR})
if(NOT pythia8)
       message(FATAL_ERROR "You need to set PYTHIA8_DIR to the install prefix of pythia.")
endif()
set(pythia8data $ENV{PYTHIA8DATA})
if(NOT pythia8data)
       message(FATAL_ERROR "You need to set PYTHIA8DATA to the path of the xmldoc directory of your pythia installation")
endif()

set(hepmc $ENV{HEPMC_PREFIX})
if(NOT hepmc)
       message(FATAL_ERROR "You need to set HEPMC_PREFIX to the install prefix of HepMC.")
else()
	set(HEPMC_PREFIX $ENV{HEPMC_PREFIX})
endif()


# Set up C++11
set(CPP11FLAGS "-std=c++11")
if (${APPLE})
    set(CPP11FLAGS "-std=c++11\ -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPP11FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")

# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{ALBERS} $ENV{FCCEDM})
find_package(alberscore COMPONENTS albers REQUIRED)
message(STATUS "Found albers-core: ${alberscore_DIR}")
find_package(fccedm REQUIRED)
message(STATUS "Found fcc-edm: ${fccedm_DIR}")
find_package(ROOT REQUIRED)
find_package(Pythia8 REQUIRED)
# set(HEPMC_PREFIX $ENV{HOME}/local)
find_package(HepMC)
find_package(Fastjet)

# set up include-directories
include_directories(
  "${ROOT_INCLUDE_DIR}"
  "${ALBERSCORE_INCLUDE_DIRS}"
  "${FCCEDM_INCLUDE_DIRS}"
  "${PYTHIA8_INCLUDE_DIRS}"
  "${HEPMC_INCLUDE}"
  "${fastjet_INCLUDE_DIR}"
  )  

# Add sub-directories
add_subdirectory(example)
add_subdirectory(example-lib)

