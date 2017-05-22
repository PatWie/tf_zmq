#include "includes/zmq.hpp"
#include "includes/tensor_msg.hpp"
#include <msgpack.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>



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
 
        char* ptr = static_cast<char*>(rawmsg.data());
        int off = 0;

        std::cout << "buf: ";
        for (int i = 0; i < rawmsg.size(); ++i)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0');
            std::cout << (static_cast<int>(ptr[i]) & 0xff) << ' ';
        }
        std::cout << std::endl;



        std::cout << "obj: ";
        std::cout << obj << std::endl;
        
      
        //  wait
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    

    }
    return 0;
}