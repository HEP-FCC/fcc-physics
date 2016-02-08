
add_executable(analysiscpp-read read.cc)

target_link_libraries(analysiscpp-read utilities datamodel podio)

install(TARGETS analysiscpp-read DESTINATION bin)
