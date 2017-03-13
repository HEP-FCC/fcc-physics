include_directories(
        ${CMAKE_SOURCE_DIR}
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

# Pay attention to the name of the dictionary...
# you want a unique name, not to pick up another .o file,
# e.g. the one of fcc-edm
REFLEX_GENERATE_DICTIONARY(fccphysics-toolsDict ${headers} SELECTION selection.xml )
add_library(fccphysics-tools SHARED ${sources} ${headers} fccphysics-toolsDict.cxx)

target_link_libraries(fccphysics-tools utilities datamodel podio ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} ${fastjet_LIBRARIES})

set_target_properties(fccphysics-tools PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS fccphysics-tools DESTINATION lib
        PUBLIC_HEADER DESTINATION include/tools)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/tools/fccphysics-toolsDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()

