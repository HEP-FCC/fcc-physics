include_directories(${CMAKE_SOURCE_DIR})

add_executable(fcc-pythia8-generate generate.cc)

if(${APPLE})
       target_link_libraries(fcc-pythia8-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES})
else()
       target_link_libraries(fcc-pythia8-generate utilities datamodel datamodelDict podio ${PYTHIA8_LIBRARIES} ${HEPMC_LIB} ${fastjet_LIBRARIES} "-lgfortran")
endif()

install(TARGETS fcc-pythia8-generate DESTINATION bin)

file(GLOB cards *.txt)
list(REMOVE_ITEM cards "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

install(FILES
  ${cards}
  DESTINATION share COMPONENT dev)

add_test(NAME generate-ee-ttbar
        COMMAND fcc-pythia8-generate ${CMAKE_CURRENT_SOURCE_DIR}/ee_ttbar.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST generate-ee-ttbar PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})

add_test(NAME generate-hh-ttbar
        COMMAND fcc-pythia8-generate ${CMAKE_CURRENT_SOURCE_DIR}/hh_ttbar.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST generate-hh-ttbar PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})

add_test(NAME generate-ZH_Zmumu_Hbb
        COMMAND fcc-pythia8-generate ${CMAKE_CURRENT_SOURCE_DIR}/ZH_Zmumu_Hbb.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST generate-ZH_Zmumu_Hbb PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH})
