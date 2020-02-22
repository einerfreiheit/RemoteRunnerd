#! /bin/bash
PU=`nproc`
ZMQ_VERSION=4.3.1
git submodule update --init --force --recursive
WORKDIR=`pwd`

libzmq_install() {
    cd $WORKDIR
    curl -L https://github.com/zeromq/libzmq/archive/v$ZMQ_VERSION.tar.gz > zeromq.tar.gz 
    tar -xvzf zeromq.tar.gz
    mkdir libzmq-build
    cmake -Hlibzmq-$ZMQ_VERSION -Blibzmq-build -DZMQ_BUILD_TESTS=OFF -DWITH_PERF_TOOL=OFF -DCMAKE_BUILD_TYPE=Release -DENABLE_DRAFTS=OFF
    cd libzmq-build
    make -j$PU
    sudo make install
}

cppzmq_install() {
    cd $WORKDIR
    cd third_party/cppzmq
    mkdir cppzmq-build 
    cmake -H. -Bcppzmq-build -DENABLE_DRAFTS=OFF -DCPPZMQ_BUILD_TESTS=OFF
    cd cppzmq-build
    make -j$PU
    sudo make install
}

runnerd_build() {
        cd $WORKDIR
        mkdir build && cd build
        cmake ..
        make -j$PU
}


libzmq_install
cppzmq_install
runnerd_build
