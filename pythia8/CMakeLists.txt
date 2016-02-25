
add_executable(fcc-pythia8-generate generate.cc)

if(${APPLE})
       target_link_libraries(fcc-pythia8-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES})
else()
       target_link_libraries(fcc-pythia8-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")
endif()

install(TARGETS fcc-pythia8-generate DESTINATION bin)

