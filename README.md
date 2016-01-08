pythiafcc
============

C++ example analysis package based on the FCC event datamodel,
pythia, and HepMC

example:
- example executable

External softare
================

This package depends on HepMC2, Pythia8 and FastJet.
You must therefore make sure that this software is installed on your machine.

If you work on lxplus, all the necessary software is picked from afs, and you can proceed to next section

If you work on another machine: 

- install the latest HepMC 2.X in an installation directory of your choice (e.g. $HOME/local). make sure HEPMC_PREFIX is pointing to this installation directory. HepMC should be configured for pythia:

     export HEPMC_PREFIX=$HOME/local
     ./configure --prefix=$HEPMC_PREFIX --with-momentum=GEV --with-length=MM

- install the latest Pythia 8 in an installation directory of your choice (e.g. $HOME/local). make sure PYTHIA8_DIR is pointing to this installation directory.
Also make sure you are configuring pythia with the version of HepMC you have just installed:
     
     export PYTHIA8_DIR=$HOME/local
     ./configure --prefix=$PYTHIA8_DIR --with-hepmc2=$HEPMC_PREFIX

- Install fastjet, and make sure FASTJET_ROOT_DIR is pointing to its installation directory


Installation
=============

- compile albers-core and source its init script to set your environment for albers.

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

    pythiafcc-generate example/ZH_Zmumu_Hbb.txt   

