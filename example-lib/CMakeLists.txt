include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

# Pay attention to the name of the dictionary...
# you want a unique name, not to pick up another .o file,
# e.g. the one of fcc-edm
ROOT_GENERATE_DICTIONARY(analysiscpp-MyAnalysisDict MyAnalysis.h LINKDEF LinkDef.h )

add_library(analysiscpp-myanalysis SHARED ${sources} ${headers} ${}analysiscpp-MyAnalysisDict.cxx)

target_link_libraries(analysiscpp-myanalysis utilities datamodel datamodelDict podio ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

install(TARGETS analysiscpp-myanalysis DESTINATION lib)

# Install the dictionary
if (${ROOT_VERSION} GREATER 6)
  install(FILES
      "${PROJECT_BINARY_DIR}/example-lib/analysiscpp-MyAnalysisDict_rdict.pcm"
      DESTINATION lib COMPONENT dev)
endif()

