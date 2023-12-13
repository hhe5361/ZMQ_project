#include <zmq.hpp>
#include <iostream>

int main() {
    zmq::context_t context(1);
    
    // Publisher 소켓 열기
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5557");
    
    // 정보 모으는 socket열기
    zmq::socket_t collector(context, ZMQ_PULL);
    collector.bind("tcp://*:5558");

    std :: cout << "server on" << std::endl;
    
    while (true) {
        zmq::message_t message;
        //계속 땡겨옴 그냥
        collector.recv(&message);
        std::string message_str(static_cast<char*>(message.data()), message.size());
        std::cout << "server: publishing update => " << message_str << std::endl;
        publisher.send(message_str.c_str(), message_str.size(), 0);
    }

    return 0;
}
