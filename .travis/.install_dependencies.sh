#!/bin/bash -e
CDIR=`pwd`
mkdir $HOME/libs
cd $HOME/libs
git clone https://github.com/zeromq/libzmq
cd libzmq
./autogen.sh
./configure
./configure --prefix=$HOME/libs/libzmq/dist
make
make install

cd $HOME/libs
git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$HOME/libs/msgpack-c/dist
make
make install
cd $CDIR
