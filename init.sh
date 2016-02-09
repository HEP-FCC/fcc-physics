
platform='unknown'
sw_afs=0
unamestr=`uname`

export ANALYSISCPP=$PWD/install

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [[ -d /afs/cern.ch/sw/lcg ]] && [[ `dnsdomainname` = 'cern.ch' ]] ; then
	#should check domain to make sure we're at CERN
	#or is this software available somewhere in Lyon? 
	sw_afs=1
	export FASTJET_ROOT_DIR=/afs/cern.ch/exp/fcc/sw/0.5/LCG_80/fastjet/3.1.1/x86_64-slc6-gcc49-opt
        export PATH=${FASTJET_ROOT_DIR}/bin:/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
	source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh
	source /afs/cern.ch/exp/fcc/sw/0.6/LCG_80/ROOT/6.04.06/x86_64-slc6-gcc49-opt/bin/thisroot.sh
	echo cmake and root taken from /afs/cern.ch/sw/lcg
    fi
    export LD_LIBRARY_PATH=$ANALYSISCPP/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$ANALYSISCPP/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform

