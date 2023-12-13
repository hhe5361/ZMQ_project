#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <sstream>
#include <string.h>

class ClientTask {
public:
    ClientTask(const std::string& id) : id(id) {}

    void run() {
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_DEALER);

        std::string identity = id;
        socket.setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket.connect("tcp://localhost:5570");

        std::cout << "Client " << identity << " started" << std::endl;

        //data가 쌓이면 이제 여기다가 쌓일거임. socket에서 오는 거 등록하는 부분임.
        zmq::pollitem_t items[] = {{socket, 0, ZMQ_POLLIN, 0}};

        int reqs = 0;
        while (true) {
            reqs++;
            std::cout << "Req #" << reqs << " sent.." << std::endl;

            std::string str = "******request #" + std::to_string(reqs);
            zmq::message_t message(str.size());
            memcpy(message.data(), str.data(), str.size());
            socket.send(message);

            std::this_thread::sleep_for(std::chrono::seconds(1));

            zmq::poll(items, 1, 1000);//두 개 이상이 오면 찍어내는 거임.
            if (items[0].revents & ZMQ_POLLIN) {
                zmq::message_t msg;
                socket.recv(&msg);
                std::string received_msg(static_cast<char*>(msg.data()), msg.size());
                std::cout << identity << " received: " << received_msg << std::endl;
            }
        }
    }

private:
    std::string id;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " client_id" << std::endl;
        return 1;
    }

    ClientTask client(argv[1]);
    client.run();

    return 0;
}
