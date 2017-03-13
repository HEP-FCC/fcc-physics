include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

# Pay attention to the name of the dictionary...
# you want a unique name, not to pick up another .o file,
# e.g. the one of fcc-edm
REFLEX_GENERATE_DICTIONARY(fccphysics-MyAnalysisDict ${headers} SELECTION selection.xml )

add_library(fccphysics-myanalysis SHARED ${sources} ${headers} fccphysics-MyAnalysisDict.cxx)

target_link_libraries(fccphysics-myanalysis utilities datamodel datamodelDict podio ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

install(TARGETS fccphysics-myanalysis DESTINATION lib)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/example-lib/fccphysics-MyAnalysisDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()

