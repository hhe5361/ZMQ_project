#include <iostream>
#include <zmq.hpp>
#include <string>

int main() {
     zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    std::cout << "Subscriber started" << std::endl;

    while (true) {
        // 메세지 받고
        zmq::message_t zmqMessage;
        subscriber.recv(zmqMessage, zmq::recv_flags::none);

        // 찍기.
        std::string receivedMessage = static_cast<char*>(zmqMessage.data());
        std::cout << "Received: " << receivedMessage << std::endl;
    }
}
