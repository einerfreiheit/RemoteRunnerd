#include "ZMQServer.h"
#include <zmq_addon.hpp>

ZMQServer::ZMQServer(const std::string& addr): addr(addr)
{
}

Message ZMQServer::receive()
{
    std::vector<zmq::message_t> received;
    zmq::recv_multipart(socket, std::back_inserter(received), zmq::recv_flags::dontwait);
    Message result;
    if (received.size() != 2) return result;
    result.identifier = received[0].to_string();
    result.content = received[1].to_string();
    return result;
}

void ZMQServer::send(const Message& reply)
{
    std::vector<zmq::message_t> toSend {2};
    toSend[0] = zmq::message_t(reply.identifier.data(), reply.identifier.size());
    toSend[1] = zmq::message_t(reply.content.data(), reply.content.size());
    zmq::send_multipart(socket, toSend, zmq::send_flags::none);

}

void ZMQServer::connect()
{
    socket = zmq::socket_t(context, zmq::socket_type::router);
    socket.setsockopt(ZMQ_ROUTER_RAW, 1);
    socket.bind(addr);
}
