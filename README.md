fcc-physics
===========

This package contains:

- various standalone C++ physics tools that you can use in your analysis, both on lxplus and your laptop
  - [pythiafcc](pythia8/generate.cc): a pythia8 executable producing events in the FCC EDM format
  - [JetClusterizer](tools/JetClusterizer.h): a fastjet interface usable in C++ and python analysis scripts to recluster jets 
- example code showing how to read and process FCC EDM events in C++:
  - [read.cc](example/read.cc): in an executable
  - [MyAnalysis](example-lib/MyAnalysis.h): in a shared library loadable in a [ROOT macro](example-lib/test_macro.py)

## Installing required software 

### On lxplus

All the necessary software has been preinstalled for you. Just make sure that you have installed [FCCSW](https://github.com/HEP-FCC/FCCSW.git) and set up your environment to use it by sourcing its [init.sh](https://github.com/HEP-FCC/podio/blob/master/init.sh) script, and skip to the next section

### On other systems

The other supported operating systems are ubuntu 14, macos X, and slc6 (other than lxplus nodes). 

You need to install: 

- The FCC packages needed to produce and read events in the FCC event data model (EDM):
  - [podio](https://github.com/HEP-FCC/podio.git): a package allowing to define event data models and to manipulate EDM events.
  - [fcc-edm](https://github.com/HEP-FCC/fcc-edm.git): definition of the FCC EDM, based on podio
- External physics software:
  - [HepMC2](http://lcgapp.cern.ch/project/simu/HepMC/download/HepMC-2.06.09.tar.gz).   
  - [pythia8](http://home.thep.lu.se/~torbjorn/pythia8/pythia8215.tgz) 
  - [fastjet](http://fastjet.fr/repo/fastjet-3.1.3.tar.gz)

**Important: make sure that your environment is properly setup before proceeding to the next section:**

  - `HEPMC_PREFIX` should point to the installation directory you have chosen for HEPMC.
  - `PYTHIA8_DIR` should point to the installation directory you have chosen for pythia8
  - `FASTJET_ROOT_DIR` should point to the installation directory you have chosen for fastjet
  
For example, assuming you have installed these three packages in `$HOME/local`, you should do:

    export HEPMC_PREFIX=$HOME/local
    export PYTHIA8_DIR=$HOME/local
    export FASTJET_ROOT_DIR=$HOME/local
    


## Installing the fcc-physics package

Before installing and everytime you want to use this software, set up your environment

    source ./init.sh

Compile:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
    make -j 4 install
    cd ..

Test: 
	
	cd build
	make test

## Generating events with pythiafcc

Example pythia8 configuration cards are available in the

## Writing your own C++ analysis code

No documentation yet, but you can already run the examples, 
and learn by looking at the code.

Create an example FCC EDM root file: 

    ${FCCEDM}/bin/fccedm-write
    
Read this file with an analysis executable:
    
    ./install/bin/analysiscpp-read    

Read this file with a python ROOT macro making use of an analysis class compiled in a shared library:

    python -i example-lib/test_macro.py 

## Generating events with pythiafcc

**COLIN add instructions for pythiafcc**
