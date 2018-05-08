ZeroMQ for TensorFlow
======================

[![Build Status](https://travis-ci.com/PatWie/tf_zmq.svg?token=EwCHGzKTTUEGS2rahMRy&branch=master)](https://travis-ci.com/PatWie/tf_zmq)

This library contains methods to send data from your C++ application **directly** into the TensorFlow graph **without** any large dependency. Unfortunately, generating data within a C++ application for TensorFlow is not envisaged and communicated as not requested for the TF core. Even the discussion about the redesign of the input pipeline seems to converge to put all data-processing into the graph. 

This is hilarious as most solutions for interesting problems are not based on a mindless iteration over a set of images. This small library helps to send data directly from your favorite C++ application (game-engine, renderer, emulator, [put here what you want]) to TensorFlow. Reading data is as simple as (see [read_tf.py](https://github.com/patwie/tf_zmq/blob/master/read_tf.py)):

```python
import tensorflow as tf
import zmq_op

image, label = zmq_op.pull('ipc:///tmp/ipc-socket-0', [tf.float32, tf.int32])
with tf.Session() as sess:
    print(sess.run([image, label]))
```

The C++ code to send data is about 45 lines in [write.cpp](https://github.com/patwie/tf_zmq/blob/master/write.cpp). It uses [ZMQ](http://zeromq.org/) for an easy distributed messaging and [msgPack](http://msgpack.org/) for a fast serialization. You are free to change the shape of each tensor over time. If you want to receive the tensors in native python without TensorFlow dependencies, you can use [read_py.py](https://github.com/patwie/tf_zmq/blob/master/read_py.py). This library supports all combinations:


| sender     | receiver                    |
| ---------- | --------------------------- |
| write.py   |  TF (read_tf.py)            |
| write.cpp  |  TF (read_tf.py)            |
| write.py   |  native python (read_py.py) |
| write.cpp  |  native python (read_py.py) |
| write.py   |  plain C++ (read.cpp)       |
| write.cpp  |  plain C++ (read.cpp)       |


Install dependencies
---------------------

You basically need two dependencies (see [.travis.yml](https://github.com/patwie/tf_zmq/blob/master/.travis.yml)).

```bash
# compile ZMQ library for c++
cd /path/to/your_lib_folder
git clone https://github.com/zeromq/libzmq
cd libzmq
./autogen.sh
./configure
./configure --prefix=/path/to/your_lib_folder/libzmq/dist
make
make install
# compile MSGPACK library for c++
cd ../
git clone https://github.com/msgpack/msgpack-c
cd msgpack-c
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/path/to/your_lib_folder/msgpack-c/dist
make
make install
```

Compile TensorFlow op and example
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

There are 2 alternative attempts for solving this problem:
- [Tensorpack](https://github.com/ppwwyyxx/tensorpack): This solution requires a python script to send data (it is based on TensorProto protobuf (currently, symbols for this are not exported by TF))
- [pull-request TF](https://github.com/tensorflow/tensorflow/pull/8728): Here you need to specify the shape dimensions in advance (also pure-python because the TensorProto dependency)

You probably do not want to link a game engine or another large codebase with the TensorFlow core using `bazel`, given compiling TensorFlow alone can be a nightmare and is only recommended for adventurer.
