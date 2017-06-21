
add_executable(fcc-pythia8-generate generate.cc)

if(${APPLE})
       target_link_libraries(fcc-pythia8-generate gflags utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES})
else()
       target_link_libraries(fcc-pythia8-generate gflags utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")
endif()

install(TARGETS fcc-pythia8-generate DESTINATION bin)

file(GLOB cards *.txt)
list(REMOVE_ITEM cards "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

install(FILES
  ${cards}
  DESTINATION share COMPONENT dev)

add_test(NAME generate-ZH_Z_Hbb
        COMMAND fcc-pythia8-generate ${CMAKE_CURRENT_SOURCE_DIR}/ee_ZH_Z_Hbb.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST generate-ZH_Z_Hbb PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
