include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

# Pay attention to the name of the dictionary...
# you want a unique name, not to pick up another .o file,
# e.g. the one of fcc-edm
ROOT_GENERATE_DICTIONARY(analysiscpp-MyAnalysisDict *.h LINKDEF LinkDef.h )

add_library(analysiscpp-myanalysis SHARED ${sources} ${headers} ${}analysiscpp-MyAnalysisDict.cxx)

target_link_libraries(analysiscpp-myanalysis utilities datamodel albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

install(TARGETS analysiscpp-myanalysis DESTINATION lib)
