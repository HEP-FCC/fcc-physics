platform='unknown'
sw_afs=0
unamestr=`uname`

export ANALYSISCPP=$PWD/install

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [ -d /afs/cern.ch/sw/lcg ]; then
	#should check domain to make sure we're at CERN
	#or is this software available somewhere in Lyon? 
	sw_afs=1
        export PATH=/afs/cern.ch/sw/lcg/contrib/CMake/2.8.9/Linux-i386/bin:${PATH}
	source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6/setup.sh
	source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.20/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
	echo cmake and root taken from /afs/cern.ch/sw/lcg
    fi
    export LD_LIBRARY_PATH=$ANALYSISCPP/lib:$LD_LIBRARY_PATH
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$ANALYSISCPP/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform

