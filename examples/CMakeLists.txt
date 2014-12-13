include_directories(
        ${CMAKE_SOURCE_DIR}/datamodel
)

add_executable(write write.cc)
add_executable(read read.cc)

target_link_libraries(write albers datamodel)
target_link_libraries(read albers datamodel)

install(TARGETS write
  # IMPORTANT: Add the write executable to the "export-set"
  EXPORT albersTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

install(TARGETS read
  # IMPORTANT: Add the read executable to the "export-set"
  EXPORT albersTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
