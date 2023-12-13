#include <zmq.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <client_id>" << std::endl;
        return 1;
    }

    zmq::context_t context(1);

    // Sub 소켓
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.connect("tcp://localhost:5557");

    // Pub 소켓
    zmq::socket_t publisher(context, ZMQ_PUSH);
    publisher.connect("tcp://localhost:5558");

    std::string clientID = argv[1];
    std::srand(std::time(0));

    while (true) {
        zmq::message_t message;
        if (subscriber.recv(&message, ZMQ_NOBLOCK)) {
            std::string message_str(static_cast<char*>(message.data()), message.size());
            std::cout << clientID << ": receive status => " << message_str << std::endl;
        } else {
            int rand_num = std::rand() % 100 + 1;
            if (rand_num < 10) {
                sleep(1);
                std::string msg = "(" + clientID + ":ON)";
                publisher.send(msg.c_str(), msg.size(), 0);
                std::cout << clientID << ": send status - activated" << std::endl;
            } else if (rand_num > 90) {
                sleep(1);
                std::string msg = "(" + clientID + ":OFF)";
                publisher.send(msg.c_str(), msg.size(), 0);
                std::cout << clientID << ": send status - deactivated" << std::endl;
            }
        }
    }

    return 0;
}
