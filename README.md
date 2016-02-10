analysis-cpp
============

C++ example analysis package based on the FCC event datamodel

example:
- example executable

example-lib:
- shared library to be used from the ROOT command line.
- pyroot macro based on this shared library


Prerequisite: before doing anything,
- compile podio and source its init script to set your environment for podio.
- compile fcc-edm and source its init script to set your environment for fcc-edm

Before compiling or using 

    source ./init.sh

Compilation:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..

Test of the executable

    ${FCCEDM}/bin/fccedm-write
    ./install/bin/analysiscpp-read    

Test of the shared library

    ${FCCEDM}/bin/fccedm-write
    python -i example-lib/test_macro.py 


