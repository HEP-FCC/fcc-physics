export PATH=/afs/cern.ch/sw/lcg/releases/LCG_72a/fastjet/3.1.1/x86_64-slc6-gcc48-opt/bin:/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.20/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh

export ANALYSISCPP=$PWD/install
export LD_LIBRARY_PATH=$ANALYSISCPP/lib:$LD_LIBRARY_PATH
