#include <iostream>
#include <zmq.hpp>
#include <string.h>
int main() {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::pull);
    subscriber.connect("tcp://localhost:5555");

    std::cout << "Pull on" << std::endl;

    //하나의 push socketㅇ서 가져오게됨. 그래서 subpub이랑 다른 점은 pub은 sub하고 있는 모든 소켓에 데이터 뿌리는 반면 , pull push는 순차적으로 분배? 하는 느낌.
    while (true) {
        zmq::message_t zmq_message;
        subscriber.recv(zmq_message, zmq::recv_flags::none);
        int message = *reinterpret_cast<int*>(zmq_message.data());
        std::cout << "Received message: " << message << std::endl;
    }

    return 0;
}
