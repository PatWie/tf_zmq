mkdir -p build
# build simple writer
echo "build simple writer"
CFLAGS=`pkg-config --cflags libzmq msgpack`
LFLAGS=`pkg-config --libs libzmq msgpack`
g++ write.cpp $CFLAGS $LFLAGS -o build/write -std=c++11
g++ read.cpp $CFLAGS $LFLAGS -o build/read -std=c++11
g++ dump.cpp $CFLAGS $LFLAGS -o build/dump -std=c++11

# build zmq_op for tensorflow
echo "build zmq_op for tensorflow"
TF_INC=$(python -c 'import tensorflow as tf; print(tf.sysconfig.get_include())')
g++ -std=c++11   --shared -fPIC -c zmq_op/zmq_pull.cc -o zmq_op/zmq_pull.cc.so -I $TF_INC $CFLAGS -std=c++11
g++ -std=c++11   -shared -o zmq_op/zmq_pull.so zmq_op/zmq_pull.cc.so -I $TF_INC -L $TF_LIB -fPIC -Wl,-rpath $TF_LIB -lcudart $LFLAGS -std=c++11

