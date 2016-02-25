add_executable(fcc-physics-read read.cc)
add_executable(fcc-physics-read-delphes read_delphes.cc)

target_link_libraries(fcc-physics-read utilities datamodel datamodelDict podio)
target_link_libraries(fcc-physics-read-delphes utilities datamodel datamodelDict podio)

install(TARGETS fcc-physics-read fcc-physics-read-delphes DESTINATION bin)

