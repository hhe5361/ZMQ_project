#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <vector>

class ServerWorker {
public:
    ServerWorker(zmq::context_t& context, int id) : context_(context), id_(id) {}

    //() operator
    void operator()() {

        zmq::socket_t worker(context_, ZMQ_DEALER);
        worker.connect("inproc://backend");//하나의 실행 프로그램 내에 있음. 즉 컴퓨터 메모리 내에서 주고 받는 거임. 어차피 같은 프로세서 안에 있으니깐
        //inmemory process약자임. 그래서 backe nd에 접근하는 거
        std::cout << "Worker#" << id_ << " started" << std::endl;

        while (true) {
            zmq::message_t identity, msg;
            worker.recv(identity, zmq::recv_flags::none);
            worker.recv(msg, zmq::recv_flags::none);

            std::string ident(static_cast<char*>(identity.data()), identity.size());
            std::string message(static_cast<char*>(msg.data()), msg.size());
            std::cout << "Worker#" << id_ << " received " << message << " from " << ident << std::endl;

            worker.send(identity, zmq::send_flags::sndmore);
            worker.send(msg, zmq::send_flags::none);
        }
    }

private:
    zmq::context_t& context_;
    int id_;
};

class ServerTask {
public:
    ServerTask(int num_server) : num_server_(num_server), context_(1) {}

    void run() {
        //front에서 들어온 요청을 router로 보내늑
        zmq::socket_t frontend(context_, ZMQ_ROUTER);
        frontend.bind("tcp://*:5570");

        zmq::socket_t backend(context_, ZMQ_DEALER);
        backend.bind("inproc://backend");//binding할 때 마찬가지로 inproc 으로 여기로 했네

        std::vector<std::thread> workers; //worker관리
        for (int i = 0; i < num_server_; ++i) {
            ServerWorker worker = ServerWorker(context_,i); //context 재사용
            workers.emplace_back(ServerWorker(context_, i));
        }
        
        //또 다 send 이런 거 하기 귀찮으니깐 zmq에서 지원하는 기능 쓰는거임.
        zmq::proxy(frontend, backend); //포워딩만 하는 역할을 한다. 즉 frontend에서 받은 socket은 backend로 이런 시긍로

        frontend.close();
        backend.close();
    }

private:
    int num_server_;
    zmq::context_t context_;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " num_servers" << std::endl;
        return 1;
    }

    int num_servers = std::stoi(argv[1]);
    ServerTask server(num_servers);
    server.run();

    return 0;
}
