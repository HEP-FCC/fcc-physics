
platform='unknown'
unamestr=`uname`

if [ -z ${FCCPHYSICS+x} ]; then
    export FCCPHYSICS=$PWD/install
    echo "FCCPHYSICS is unset, setting to $FCCPHYSICS";
fi

if [[ "$unamestr" == 'Linux' ]]; then
    platform='Linux'
    if [[ -d /cvmfs/fcc.cern.ch/sw ]]; then
        #should check domain to make sure we're at CERN
        #or is this software available somewhere in Lyon?
        source /cvmfs/fcc.cern.ch/sw/0.8.1/init_fcc_stack.sh
    fi
elif [[ "$unamestr" == 'Darwin' ]]; then
    platform='Darwin'
    export DYLD_LIBRARY_PATH=$FCCPHYSICS/lib:$HEPMC_PREFIX/lib:$FASTJET_ROOT_DIR/lib:$PYTHIA8_DIR/lib:$DYLD_LIBRARY_PATH
fi
echo platform detected: $platform
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$FCCEDM/include/datamodel
export PATH=$FCCPHYSICS/bin:$FASTJET_ROOT_DIR/bin:$PATH
export PYTHIA8DATA=${PYTHIA8_DIR}/share/Pythia8/xmldoc
export PYTHONPATH=$PODIO/python:$PYTHONPATH
