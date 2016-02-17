
add_executable(analysiscpp-read read.cc)
add_executable(analysiscpp-read-delphes read_delphes.cc)

target_link_libraries(analysiscpp-read utilities datamodel datamodelDict podio)
target_link_libraries(analysiscpp-read-delphes utilities datamodel datamodelDict podio)


install(TARGETS analysiscpp-read analysiscpp-read-delphes DESTINATION bin)
