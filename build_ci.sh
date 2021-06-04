#! /bin/bash
PU=`nproc`
WORKDIR=`pwd`

runnerd_build() {
        cd $WORKDIR
        mkdir build
        cd build
        cmake ..
        make -j$PU
}
runnerd_build
