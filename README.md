pythiafcc
============

C++ example analysis package based on the FCC event datamodel,
pythia, and HepMC

example:
- example executable


Prerequisite, before doing anything: 

- install the latest HepMC 2.X in an installation directory of your choice (e.g. $HOME/local). make sure HEPMC_PREFIX is pointing to this installation directory. HepMC should be configured for pythia:

     export HEPMC_PREFIX=$HOME/local
     ./configure --prefix=$HEPMC_PREFIX --with-momentum=GEV --with-length=MM

- install the latest Pythia 8 in an installation directory of your choice (e.g. $HOME/local). make sure PYTHIA8_DIR is pointing to this installation directory.
Also make sure you are configuring pythia with the version of HepMC you have just installed:
     
     export PYTHIA8_DIR=$HOME/local
     ./configure --prefix=$PYTHIA8_DIR --with-hepmc2=$HEPMC_PREFIX

- In case you're not working on lxplus, install fastjet, make sure the fastjet-config executable is in your PATH, and make sure the fastjet shared libraries are in your LD_LIBRARY_PATH (on linux) or in your DYLD_LIBRARY_PATH (macOS).

- compile albers-core and source its init script to set your environment for albers.

- compile fcc-edm and source its init script to set your environment for fcc-edm


Before compiling or using on lxplus:

    source ./init.sh

Before compiling or using on mac os: 

    source ./init_macos.sh

Before compiling or using on linux (tested on Ubuntu):

    source ./init_linux.sh

Compilation:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..

Test of the executable

    ./install/bin/pythiafcc-generate   

