
platform='unknown'
sw_afs=0
unamestr=`uname`

if [ -z ${FCCPHYSICS+x} ]; then 
    export FCCPHYSICS=$PWD/install
    echo "FCCPHYSICS is unset, setting to $FCCPHYSICS";
fi

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [[ -d /afs/cern.ch/sw/lcg ]] && [[ `dnsdomainname` = 'cern.ch' ]] ; then
	#should check domain to make sure we're at CERN
	#or is this software available somewhere in Lyon? 
	sw_afs=1
	export FASTJET_ROOT_DIR=/afs/cern.ch/exp/fcc/sw/0.5/LCG_80/fastjet/3.1.1/x86_64-slc6-gcc49-opt
	export PYTHIA8_DIR=/afs/cern.ch/exp/fcc/sw/0.6/LCG_80/MCGenerators/pythia8/212/x86_64-slc6-gcc49-opt
	export HEPMC_PREFIX=/afs/cern.ch/exp/fcc/sw/0.6/LCG_80/HepMC/2.06.09/x86_64-slc6-gcc49-opt
	export FCCEDM=/afs/cern.ch/exp/fcc/sw/0.6/fcc-edm/0.2/x86_64-slc6-gcc49-opt/
	export PODIO=/afs/cern.ch/exp/fcc/sw/0.6/podio/0.2/x86_64-slc6-gcc49-opt
        export PATH=/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
	source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh
	source /afs/cern.ch/exp/fcc/sw/0.6/LCG_80/ROOT/6.04.06/x86_64-slc6-gcc49-opt/bin/thisroot.sh
	echo software taken from /afs/cern.ch/sw/lcg
    fi
    export LD_LIBRARY_PATH=$FCCPHYSICS/lib:$FCCEDM/lib:$PODIO/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$FCCPHYSICS/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform
export PATH=$FCCPHYSICS/bin:$FASTJET_ROOT_DIR/bin:$PATH
export PYTHIA8DATA=${PYTHIA8_DIR}/share/Pythia8/xmldoc
