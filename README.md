ZeroMQ for TensorFlow
======================

[![Build Status](https://travis-ci.com/PatWie/tf_zmq.svg?token=EwCHGzKTTUEGS2rahMRy&branch=master)](https://travis-ci.com/PatWie/tf_zmq)

This is about directly sending data from C++ to Tensorflow. Unfortunately, generating data within a c++ application for TF is not envisaged and communicated as not request for the TF core. Even the discussion about the redesign of the input pipeline seems to converge to put everything into the graph. 

This is hilarious as most solutions for interesting problems are not based on a mindless iteration over set of images. This small library helps to sending data directly from your favourite C++ application (game-engine, renderer, emulator, [put here what you want]) to TensorFlow. Reading data is as simple as:

```python
import tensorflow as tf
import zmq_op

image, label = zmq_op.pull('ipc:///tmp/ipc-socket-0', [tf.float32, tf.int32])
with tf.Session() as sess:
    print(sess.run([image, label]))
```

The C++ code is about 50 lines in [write.cpp](https://github.com/patwie/tf_zmq/blob/master/write.cpp). It uses [ZMQ](http://zeromq.org/) for a easy distributed messaging and [msgPack](http://msgpack.org/) for a fast serialization. You are free to change the shape of each tensor over time.


install dependencies
---------------------

You basically need two dependencies (see [.travis.yml](https://github.com/patwie/tf_zmq/blob/master/.travis.yml)).

```bash
# compile ZMQ
cd /path/to/your_lib_folder
git clone https://github.com/zeromq/libzmq
cd libzmq
./autogen.sh
./configure
./configure --prefix=/path/to/your_lib_folder/libzmq/dist
make
make install
# compile MSGPACK
cd ../
git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/path/to/your_lib_folder/msgpack-c/dist
make
make install
```

compile tensorflow op and example
----------------------
```bash
export PKG_CONFIG_PATH=/path/to/your_lib_folder/libzmq/dist/lib/pkgconfig/:$PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/path/to/your_lib_folder/msgpack-c/dist/lib/pkgconfig/:$PKG_CONFIG_PATH
git clone https://github.com/PatWie/tf_zmq.git
cd tf_zmq
./compile.sh
```


related projects:
-------------

There have been 2 attemps for solving this problem:
- [Tensorpack](https://github.com/ppwwyyxx/tensorpack) this solution requires a python script to send data as it is based on TensorProto protobuf (currently not exported by TF)
- [pull-request TF](https://github.com/tensorflow/tensorflow/pull/8728) here you need to specify the shape dimensions in advance also based on TensorProto

You probably do not want to link a game engine with the TF core.

