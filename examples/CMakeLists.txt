include_directories(
        ${CMAKE_SOURCE_DIR}/datamodel
)

add_executable(writefcc write.cc)
add_executable(readfcc read.cc)

target_link_libraries(writefcc utilities datamodel albers)
target_link_libraries(readfcc utilities datamodel albers)

install(TARGETS writefcc readfcc
  # IMPORTANT: Add the write executable to the "export-set"
  EXPORT fccedmTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

#install(TARGETS read
#  # IMPORTANT: Add the read executable to the "export-set"
#  EXPORT fccedmTargets
#  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
