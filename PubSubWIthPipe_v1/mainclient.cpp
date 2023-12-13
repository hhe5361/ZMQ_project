#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <string.h>
int main() {
    srand(time(NULL));

    zmq::context_t context(1);

    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5551");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); //sub socket askii? 

    zmq::socket_t pushsocket(context, ZMQ_PUSH);//push socket
    pushsocket.connect("tcp://localhost:5552");

    zmq::pollitem_t items[] = {{(subscriber), 0, ZMQ_POLLIN, 0}};
    std::cout << "client on\n";

    while (true) {
        zmq::poll(items, 1, 100);
        //:poll(zmq_pollitem_t *items_, size_t nitems_, long timeout_)임. 100초 기다리는 ㄱ ㅓ
        if (items[0].revents & ZMQ_POLLIN) {
            int message;
            subscriber.recv(&message, sizeof(int));
            std::cout << "I: received message " << message << std::endl;
        } else {
            int random = (rand() % 100) + 1;
            std::this_thread::sleep_for(std::chrono::seconds(1));

            pushsocket.send(&random, sizeof(int));
            std::cout << "I: sending message " << random << std::endl;
        }
    }

    return 0;
}
