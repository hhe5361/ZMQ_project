#include <iostream>
#include <zmq.hpp>
#include <string>
#include <thread>

int main() {
     zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5555");

    std::cout << "Publisher started" << std::endl;

    while (true) {
        int zipcode = rand() % 100;
        int temperature = (rand() % 50) - 20;
        std::string message = "zipcode : " + std::to_string(zipcode) + " and temperature : " + std::to_string(temperature);

        publisher.send(zmq::buffer(message,message.length()), zmq::send_flags::none);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
