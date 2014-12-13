include_directories(
        ${CMAKE_SOURCE_DIR}/datamodel
)

file(GLOB sources *.cc)
file(GLOB headers *.h)

add_library(utilities SHARED ${sources} ${headers} )
target_link_libraries(utilities datamodel albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

set_target_properties(utilities PROPERTIES
  PUBLIC_HEADER "${headers}")

install(TARGETS utilities
  # IMPORTANT: Add the albers library to the "export-set"
  EXPORT fccedmTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/utilities"
    COMPONENT dev)
