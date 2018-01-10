
add_executable(hzha  steering.f hcha03.f  hzha03.f int03.f pythia-6.4.28.f)
target_link_libraries(hzha PUBLIC  -L/afs/cern.ch/sw/lcg/external/cernlib/2006b/x86_64-slc5-gcc43-opt/lib/ -lkernlib -lpacklib -lmathlib -lkernlib-shift)

add_executable(hzhacpp hzha.cpp readcards.f hcha03.f  hzha03.f int03.f pythia-6.4.28.f)
target_link_libraries(hzhacpp PUBLIC -L/afs/cern.ch/sw/lcg/external/cernlib/2006b/x86_64-slc5-gcc43-opt/lib/ -lkernlib -lpacklib -lmathlib -lkernlib-shift ${HEPMC_LIBRARIES})

message(${HEPMC_LIBRARIES})

set_property(TARGET hzhacpp PROPERTY LINKER_LANGUAGE Fortran)

install(TARGETS hzha hzhacpp DESTINATION bin)

file(GLOB cards *.cards)
install(FILES
  ${cards}
  DESTINATION share COMPONENT dev)


