add_executable(fcc-physics-read read.cc)
add_executable(fcc-physics-read-delphes read_delphes.cc)

target_link_libraries(fcc-physics-read utilities datamodel datamodelDict podio)
target_link_libraries(fcc-physics-read-delphes utilities datamodel datamodelDict podio)

install(TARGETS fcc-physics-read fcc-physics-read-delphes DESTINATION bin)

add_test(NAME read
    COMMAND fcc-physics-read ${CMAKE_BINARY_DIR}/ZH_Zmumu_Hbb.root
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST read PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
set_property(TEST read PROPERTY DEPENDS generate-ZH_Zmumu_Hbb)
