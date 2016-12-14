
add_executable(fcc-pythia8-hist pythia8-hist.cc)

       target_link_libraries(fcc-pythia8-hist utilities ${PYTHIA8_LIBRARIES})

install(TARGETS fcc-pythia8-hist DESTINATION bin)

file(GLOB scripts *.py hh*.txt Makefile)

install(FILES
  ${scripts}
  DESTINATION min-bias)

