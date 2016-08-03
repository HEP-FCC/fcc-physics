include_directories(${CMAKE_SOURCE_DIR})

add_executable(fcc-physics-read read.cc)
add_executable(fcc-physics-read-delphes read_delphes.cc)
add_executable(fcc-physics-read-hepmc convert_hepmc.cc)

target_link_libraries(fcc-physics-read utilities datamodel datamodelDict podio)
target_link_libraries(fcc-physics-read-delphes utilities datamodel datamodelDict podio)
target_link_libraries(fcc-physics-read-hepmc utilities datamodel datamodelDict podio HepMC)

install(TARGETS fcc-physics-read fcc-physics-read-delphes fcc-physics-read-hepmc DESTINATION bin)

add_test(NAME read
    COMMAND fcc-physics-read ${CMAKE_BINARY_DIR}/ZH_Zmumu_Hbb.root
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST read PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
set_property(TEST read PROPERTY DEPENDS generate-ZH_Zmumu_Hbb)

add_test(NAME read_hepmc
        COMMAND fcc-physics-read-hepmc /afs/cern.ch/exp/fcc/sw/0.7/testsamples/testHepMCborders.dat FCCTest.root)
set_property(TEST read_hepmc PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
