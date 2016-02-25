add_executable(analysiscpp-read read.cc)
add_executable(analysiscpp-read-delphes read_delphes.cc)
add_executable(pythiafcc-generate generate.cc)

target_link_libraries(analysiscpp-read utilities datamodel datamodelDict podio)
target_link_libraries(analysiscpp-read-delphes utilities datamodel datamodelDict podio)

if(${APPLE})
       target_link_libraries(pythiafcc-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES})
else()
       target_link_libraries(pythiafcc-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")
endif()


install(TARGETS analysiscpp-read analysiscpp-read-delphes pythiafcc-generate DESTINATION bin)

