
add_executable(pythiafcc-generate generate.cc)

target_link_libraries(pythiafcc-generate utilities datamodel albers ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")

install(TARGETS pythiafcc-generate DESTINATION bin)
