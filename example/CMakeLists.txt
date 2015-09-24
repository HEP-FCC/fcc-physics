
add_executable(pythiafcc-generate generate.cc)

if(${APPLE})
       target_link_libraries(pythiafcc-generate utilities datamodel albers ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES})
else()
       target_link_libraries(pythiafcc-generate utilities datamodel albers ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")
endif()

install(TARGETS pythiafcc-generate DESTINATION bin)
