#ifndef ZMQSERVER_H
#define ZMQSERVER_H
#include <zmq.hpp>

struct Message {
    std::string identifier;
    std::string content;
};

class ZMQServer
{
public:
    explicit ZMQServer(const std::string& addr);
    ~ZMQServer() = default;
    Message receive();
    void send(const Message& reply);
    void connect();
private:
    zmq::context_t context;
    zmq::socket_t socket;
    std::string addr;
};

#endif // ZMQSERVER_H
