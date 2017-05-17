//File: zmq_pull.cc
//Author: Patrick Wieschollek <mail@patwie.com>

#include <string>
#include <memory>
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/common_shape_fns.h"

#include "../includes/zmq.hpp"
#include "../includes/tensor_msg.hpp"

using namespace std;
using namespace tensorflow;

REGISTER_OP("ZMQPull")
    .Attr("address: string")
    .Attr("types: list(type) >= 1")
    .Output("output: types")
    .SetShapeFn(shape_inference::UnknownShape)
    .SetIsStateful()
    .Doc(R"doc(
Produce list of tensors when reading from ZMQ pipe.

Data serialized by a custom `tensor_msg` within C++ applications can be sent
over a socket directly to a TF graph. Length of list of tensors has to be
known beforehand. This only supports tf.float32, tf.int32, tf.double32.

address: string containing the addess of the socket.
types: list of data types, describing the incoming data.
)doc");

class zmq_pipe
{
public:
  zmq_pipe(std::string address) : context(1), socket(context, ZMQ_PULL){
    socket.bind(address);
  }
  zmq::context_t context;
  zmq::socket_t socket;
};


class ZMQPull: public OpKernel {
 public:
  explicit ZMQPull(OpKernelConstruction* context) : OpKernel(context) {
    // get pipe name
    string address;
    OP_REQUIRES_OK(context, context->GetAttr("address", &address));
    // setup ZMQ connection
    pipe.reset(new zmq_pipe(address));

  }

  void Compute(OpKernelContext* context) override {
    // raw message packed with msgPack
    zmq::message_t rawmsg;

    // read and unpack message using our custom tensor_msg class
    pipe->socket.recv (&rawmsg);
    msgpack::object_handle oh = msgpack::unpack(static_cast<char*>(rawmsg.data()), rawmsg.size());

    // list of tensors
    std::vector<tensor_msg> tensors;

    // convert
    msgpack::object obj = oh.get();
    obj.convert(tensors);

    // generate each single tensor from entire list
    for (int i = 0; i < tensors.size(); ++i)
    {
      // generate shape
      TensorShape output_shape;
      for(int d : tensors[i].shape_)
        output_shape.AddDim(d);
 
      // fill data
      Tensor* output = nullptr;
      OP_REQUIRES_OK(context,context->allocate_output(i, output_shape, &output));

      if(tensors[i].type_ == 0){
        // case: type tf.int32
        auto flat = output->flat<int>();
        std::copy_n((int*) tensors[i].data_.ptr,
                    tensors[i].size(),
                    flat.data());
      }
      else if(tensors[i].type_ == 1){
        // case: type tf.float32
        auto flat = output->flat<float>();
        std::copy_n((float*) tensors[i].data_.ptr,
                    tensors[i].size(),
                    flat.data());
      }
      else if(tensors[i].type_ == 2){
        // case: type tf.float64 (double)
        auto flat = output->flat<double>();
        std::copy_n((double*) tensors[i].data_.ptr,
                    tensors[i].size(),
                    flat.data());
      }
    }
  }
 private:

  std::string address;
  unique_ptr<zmq_pipe> pipe;

};

REGISTER_KERNEL_BUILDER(Name("ZMQPull").Device(DEVICE_CPU), ZMQPull);
