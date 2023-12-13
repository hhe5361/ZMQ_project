#include <iostream>
#include <string.h>
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    std::cout << "Connection 1:1 chat server.." << std::endl;
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    while(1){ 
        std::string request;

        std :: cout << "전송할 문자를 입력해주세요:";
        std ::getline(std::cin,request);

        if(request == "q") break;
        else{
            std::cout << "Sending request " << request << "..." << std::endl;
            socket.send(zmq::buffer(request,request.length()), zmq::send_flags::none);//긁적 고쳐야 함. 동작 이상하게 하는데 

            zmq::message_t message;
            zmq::recv_result_t result = socket.recv(message, zmq::recv_flags::none);
            if (result.has_value()) {
                std::cout << "응답 받음!" << request << ": [" << std::string(static_cast<char*>(message.data()), message.size()) << "]" << std::endl;
            } else {
                std::cerr << "Error" << std::endl;
            }
        }
    }

    std :: cout << "client down";
    return 0;
}
