pythiafcc
============

C++ example analysis package based on the FCC event datamodel,
pythia, and HepMC

example:
- example executable


Prerequisite: before doing anything,

- install HepMC, and make sure that HEPMC_PREFIX is pointing to the installation directory of HepMC. HepMC should be configured for pythia:

     ./configure --prefix=$HEPMC_PREFIX --with-momentum=GEV --with-length=MM

- install pythia8, and make sure that PYTHIA8_DIR is pointing to the installation prefix of pythia8

- compile albers-core and source its init script to set your environment for albers.

- compile fcc-edm and source its init script to set your environment for fcc-edm

Before compiling or using on lxplus:

    source init.sh

Before compiling or using on mac os: 

    source init_macos.sh

Before compiling or using on linux (tested on Ubuntu):

    source init_linux.sh

Compilation:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..

Test of the executable

    ./install/bin/pythiafcc-generate   

