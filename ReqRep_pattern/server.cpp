#include <iostream>
#include <thread>
#include <zmq.hpp>
#include <string.h>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    std::cout << "Server activate" << std::endl;

    while (true) {
        zmq::message_t message;
        zmq :: recv_result_t result =  socket.recv(message, zmq::recv_flags::none);

        std::string receivedMessage = static_cast<char*>(message.data());
        std::cout << "Received Request: " <<receivedMessage << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string response = "you : " + receivedMessage + "\ncom : greate";
        socket.send(zmq::buffer(response.c_str(), response.size()), zmq::send_flags::none);
    }

    return 0;
}
