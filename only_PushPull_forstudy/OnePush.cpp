#include <iostream>
#include <zmq.hpp>
#include <thread>

int main() {
    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::push);
    publisher.bind("tcp://*:5555");

    std::cout << "Push on" << std::endl;

    //push에서는 순차적으로 연결된 socket에 메세지 전송. 
    while (true) {
        int message = rand() % 100;
        std::cout << "Pushing message: " << message << std::endl;
        publisher.send(zmq::buffer(&message, sizeof(int)), zmq::send_flags::none);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
