analysis-cpp
============

C++ example analysis package based on the FCC event datamodel

example:
- example executable

example-lib:
- shared library to be used from the ROOT command line.
- ROOT macro based on this shared library


Prerequisite: before doing anything,
- compile albers and source its init script to set your environment for albers.
- compile fcc-edm and source its init script to set your environment for fcc-edm

Before compiling or using on lxplus:

    source init.sh

Before compiling or using on mac os: 

    source init_macos.sh

Compilation:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..

Test of the executable

    ${FCCEDM}/bin/fccedm-write
    ./install/bin/pythiafcc-read    

Test of the shared library

    ${FCCEDM}/bin/fccedm-write
    root example-lib/test_macro.C
    


