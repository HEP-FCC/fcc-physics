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


# Set up C++14
set(CPPSTDFLAGS "-std=c++14")
if (${APPLE})
    set(CPPSTDFLAGS "-std=c++14\ -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DDROP_CGAL ${CPPSTDFLAGS} -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")

# Make sure we find the Find*.cmake functions distributed with this package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{ROOTSYS} $ENV{PODIO} $ENV{FCCEDM})

find_package(podio REQUIRED HINTS $ENV{PODIO})
message(STATUS "Found podio: ${podio_DIR}")
# Make sure the library is found.
# Not the case if LD_LIBRARY_PATH is wrong
find_library(PODIOLIB podio)
if (NOT PODIOLIB)
    message(FATAL_ERROR "libpodio.so(dylib) cannot be found dynamically. Make sure you have sourced PODIO init*.sh file to set up your environment to use PODIO")
endif()
link_directories(${podio_LIBRARY_DIR})

find_package(fccedm REQUIRED HINTS $ENV{FCCEDM})
message(STATUS "Found fcc-edm: ${fccedm_DIR}")
# find_library(FCCEDMLIB datamodel HINTS ${fccedm_LIBRARIES})
# message("FCCEDM ${FCCEDMLIB}")

# enable testing infrastructure
include(CTest)

find_package(ROOT REQUIRED)
find_package(HepMC)
find_package(Pythia8 REQUIRED)
find_package(Fastjet)

# set up include-directories
include_directories(
  "${ROOT_INCLUDE_DIRS}"
  "${podio_INCLUDE_DIRS}"
  "${FCCEDM_INCLUDE_DIRS}"
  "${PYTHIA8_INCLUDE_DIRS}"
  "${HEPMC_INCLUDE}"
  "${fastjet_INCLUDE_DIR}"
  )

include(${ROOT_USE_FILE})

# Add sub-directories
add_subdirectory(example)
add_subdirectory(example-lib)
add_subdirectory(tools)
add_subdirectory(pythia8)
