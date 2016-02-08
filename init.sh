export FASTJET_ROOT_DIR=/afs/cern.ch/exp/fcc/sw/0.5/LCG_80/fastjet/3.1.1/x86_64-slc6-gcc49-opt
export PATH=/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh
source /afs/cern.ch/exp/fcc/sw/0.6/LCG_80/ROOT/6.04.06/x86_64-slc6-gcc49-opt/bin/thisroot.sh

export PYTHIA8_DIR=/afs/cern.ch/exp/fcc/sw/0.6/LCG_80/MCGenerators/pythia8/212/x86_64-slc6-gcc49-opt
export HEPMC_PREFIX=/afs/cern.ch/exp/fcc/sw/0.6/LCG_80/HepMC/2.06.09/x86_64-slc6-gcc49-opt
export FCC_EDM_DIR=/afs/cern.ch/exp/fcc/sw/0.5/fcc-edm/0.2/x86_64-slc6-gcc49-opt

export PYTHIA8DATA=${PYTHIA8_DIR}/share/Pythia8/xmldoc
export PYTHIAFCC=$PWD/install
export LD_LIBRARY_PATH=$PYTHIAFCC/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:FCC_EDM_DIR/lib:$LD_LIBRARY_PATH
export PATH=$PYTHIAFCC/bin:$FASTJET_ROOT_DIR/bin:$PATH

