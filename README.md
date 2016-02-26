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

**If you're working on lxplus, all the necessary software has been preinstalled for you and you can just proceed with [installing the fcc-physics package](#install_fcc).**

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
    


## <a name="install_fcc"></a>Installing the fcc-physics package

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

Example pythia8 configuration cards are available in the [pythia8]
(pythia8) directory. 

To generate e+e to ZH with Z to mumu and H to b bbar at 240 GeV, do:

    fcc-pythia8-generate pythia8/ZH_Zmumu_Hbb.txt 
   
The events are stored in the FCC EDM format in `example.root`

Open this file with root to see which collections of objects have been stored: 

    root example.root
    events->Print()
    
You should get a printout like: 

	******************************************************************************
	*Tree    :events    : Events tree                                            *
	*Entries :    10000 : Total =        38573156 bytes  File  Size =   10281281 *
	*        :          : Tree compression factor =   3.75                       *
	******************************************************************************
	*Br    0 :GenParticle : Int_t GenParticle_                                   *
	*Entries :    10000 : Total  Size=     106318 bytes  File Size  =      26175 *
	*Baskets :        4 : Basket Size=      32000 bytes  Compression=   3.07     *
	*............................................................................*
	*Br    1 :GenParticle.Core.Status : UInt_t Status[GenParticle_]              *
	*Entries :    10000 : Total  Size=    2893248 bytes  File Size  =      56959 *
	*Baskets :       92 : Basket Size=      32000 bytes  Compression=  50.76     *
	*............................................................................*
	*Br    2 :GenParticle.Core.Charge : Int_t Charge[GenParticle_]               *
	*Entries :    10000 : Total  Size=    2893248 bytes  File Size  =      56959 *
	*Baskets :       92 : Basket Size=      32000 bytes  Compression=  50.76     *
	*............................................................................*

Now plot the charge of the stable generated particles: 

    events->Draw("GenParticle.Core.Charge", "GenParticle.Core.Status==1")
    

## Writing your own C++ analysis code

No documentation yet, but you can already run the examples, 
and learn by looking at the code.
    
Read the file produced earlier with pythia8, using an analysis executable:
    
    ./install/bin/fcc-physics-read    

Read this file with a python ROOT macro making use of an analysis class compiled in a shared library:

    python -i example-lib/test_macro.py 


    