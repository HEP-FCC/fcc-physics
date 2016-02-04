export FASTJET_ROOT_DIR=/afs/cern.ch/exp/fcc/sw/0.5/LCG_80/fastjet/3.1.1/x86_64-slc6-gcc49-opt
export PATH=/afs/cern.ch/sw/lcg/releases/LCG_80/fastjet/3.1.1/x86_64-slc6-gcc49-opt/bin:/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh
source /afs/cern.ch/exp/fcc/sw/0.6/LCG_80/ROOT/6.04.06/x86_64-slc6-gcc49-opt/bin/thisroot.sh

export ANALYSISCPP=$PWD/install
export LD_LIBRARY_PATH=$ANALYSISCPP/lib:$LD_LIBRARY_PATH
