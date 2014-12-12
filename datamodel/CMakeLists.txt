include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/datamodel
)

file(GLOB sources src/*.cc datamodel/*.h)
# file(GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
set(headers datamodel/*.h)

# list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/datamodel/LinkDef.h  )
message(DICTIONARY ${headers})
ROOT_GENERATE_DICTIONARY(ExampleDict ${headers} LINKDEF LinkDef.h )

add_library(datamodel SHARED ${sources} ${}ExampleDict.cxx)
target_link_libraries(datamodel albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

