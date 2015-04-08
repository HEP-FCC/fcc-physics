include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

# Pay attention to the name of the dictionary...
# you want a unique name, not to pick up another .o file,
# e.g. the one of fcc-edm
# ROOT_GENERATE_DICTIONARY(analysiscpp-toolsDict *.h LINKDEF LinkDef.h )

# add_library(analysiscpp-tools SHARED ${sources} ${headers} ${}analysiscpp-toolsDict.cxx)

add_library(analysiscpp-tools SHARED ${sources} ${headers} )

target_link_libraries(analysiscpp-tools utilities datamodel albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES} ${fastjet_LIBRARIES})

message("${fastjet_LIBRARY}")

install(TARGETS analysiscpp-tools DESTINATION lib)
