configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

file(GLOB sources *.cc)
file(GLOB headers *.h)

add_library(albers SHARED ${sources} ${headers} config.h.in)
target_link_libraries(albers ${ROOT_LIBRARIES} ${ROOT_COMPONENT_LIBRARIES})

set_target_properties(albers PROPERTIES
  PUBLIC_HEADER "CollectionBase.h;${CMAKE_CURRENT_BINARY_DIR}/config.h")

install(TARGETS albers
  # IMPORTANT: Add the albers library to the "export-set"
  EXPORT albersTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/albers"
    COMPONENT dev)
