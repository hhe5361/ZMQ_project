#include <iostream>
#include <zmq.hpp>
#include <chrono> 
#include <thread>

int main() {
    zmq::context_t context(1);

    //pub socket bind
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5551");

    //pull, bind 클라이언트 들로부터 데이터 가져오는 거임.
    zmq::socket_t collector(context, ZMQ_PULL);
    collector.bind("tcp://*:5552");

    std :: cout << "server on" << std::endl;

    int total = 0;
    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        int zmq_message;

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

        if (elapsed_time >= 10) {
            std::cout << "I: publishing total " << total << std::endl;
            publisher.send(&total,sizeof(int));
            total = 0;
            start_time = std::chrono::steady_clock::now();
        }
        collector.recv(&zmq_message,sizeof(int)); // collector로부터 message 가져옴.
        total += zmq_message;
        std::cout << "I: received data " << zmq_message << std::endl;
    }

    return 0;
}
