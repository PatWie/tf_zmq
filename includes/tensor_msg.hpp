#ifndef tensor_msg_HPP
#define tensor_msg_HPP

#include <msgpack.hpp>
#include <vector>
#include <iostream>

/**
 * @brief Represent a tiny class for serializing tensors
 * @details each tensor consists of information about the type,
 *          the shape and the underlying tensor data
 */
class tensor_msg {
public:
    // type of message (0=int, 1=float, 2=double)
    unsigned int type_;
    // dimensions (length of each axis)
    std::vector<unsigned int> shape_;
    // raw data
    msgpack::type::raw_ref data_;

  tensor_msg(){}

  tensor_msg(std::vector<unsigned int> shape, int* data){
    init(shape, (char*) data, sizeof(int), 0);
  }

  tensor_msg(std::vector<unsigned int> shape, float* data){
    init(shape, (char*) data, sizeof(float), 1);
  }

  tensor_msg(std::vector<unsigned int> shape, double* data){
    init(shape, (char*) data, sizeof(double), 2);
  }

  void init(std::vector<unsigned int> shape, void* data, int size, unsigned int type){
    shape_ = shape;
    type_ = type;
    int elements = 1;
    for(auto &i : shape)
      elements *= i;
    data_.ptr = (char*) data;
    data_.size = size * elements;
  }

  // number of entries
  unsigned int size(){
    // templating seems no to be possible as we gonna read the data on-the-fly later
    if (type_ == 0)
      return data_.size / sizeof(int);
    if (type_ == 1)
      return data_.size / sizeof(float);
    if (type_ == 2)
      return data_.size / sizeof(double);
  }

  ~tensor_msg(){}
  MSGPACK_DEFINE(type_, shape_, data_);
};

#endif