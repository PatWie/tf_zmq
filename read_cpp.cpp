#include "includes/zmq.hpp"
#include "includes/tensor_msg.hpp"
#include <msgpack.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>


void PrintData(std::vector<tensor_msg> &msg){
    std::cout << "Number of Tensors: "<< msg.size() << std::endl;
    
    int c = 0;
    for(auto &tensor : msg){
        std::cout << "  index: " << (c++) << std::endl;
        std::cout << "  shape: " << " ";
        for(int d : tensor.shape_)
            std::cout << d << " ";
    }
    std::cout << std::endl;
      
}

int main () {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_PULL);
    socket.bind ("ipc:///tmp/ipc-socket-0");

    while (true) {
        zmq::message_t rawmsg;

        // message reader
        socket.recv (&rawmsg);
        msgpack::object_handle oh = msgpack::unpack(static_cast<char*>(rawmsg.data()), rawmsg.size());

        // list of tensors
        std::vector<tensor_msg> tensors;

        // read and convert
        msgpack::object obj = oh.get();
        obj.convert(tensors);
       

        // display information
        PrintData(tensors);
      
        //  wait
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    

    }
    return 0;
}