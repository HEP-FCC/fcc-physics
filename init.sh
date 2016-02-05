platform='unknown'
sw_afs=0
unamestr=`uname`

export PYTHIAFCC=$PWD/install

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [[ -d /afs/cern.ch/sw/lcg ]] && [[ `dnsdomainname` = 'cern.ch' ]] ; then
	#should check domain to make sure we're at CERN
	#or is this software available somewhere in Lyon? 
	sw_afs=1
	export FASTJET_ROOT_DIR=/afs/cern.ch/sw/lcg/releases/LCG_72a/fastjet/3.1.1/x86_64-slc6-gcc48-opt
	export PYTHIA8_DIR=/afs/cern.ch/sw/lcg/releases/LCG_72a/MCGenerators/pythia8/210/x86_64-slc6-gcc48-opt/
	export HEPMC_PREFIX=/afs/cern.ch/sw/lcg/releases/LCG_72a/HepMC/2.06.09/x86_64-slc6-gcc48-opt/
        export PATH=/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
	source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6/setup.sh
	source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.20/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
	echo cmake and root taken from /afs/cern.ch/sw/lcg
    fi
    export LD_LIBRARY_PATH=$PYTHIAFCC/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$PYTHIAFCC/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform
export PATH=$PYTHIAFCC/bin:$FASTJET_ROOT_DIR/bin:$PATH
export PYTHIA8DATA=${PYTHIA8_DIR}/share/Pythia8/xmldoc
