include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/datamodel
)

file(GLOB sources src/*.cc datamodel/*.h)
file(GLOB headers datamodel/*.h)

ROOT_GENERATE_DICTIONARY(ExampleDict datamodel/*.h LINKDEF LinkDef.h )

add_library(datamodel SHARED ${sources} ${}ExampleDict.cxx)
target_link_libraries(datamodel albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

set_target_properties(datamodel PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS datamodel
  # IMPORTANT: Add the albers library to the "export-set"
  EXPORT datamodelTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/datamodel"
    COMPONENT dev)
