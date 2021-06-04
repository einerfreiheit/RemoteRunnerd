#include "TCPEndpoint.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sstream>

namespace remote_runnerd {
namespace {

const std::string HOST = "127.0.0.1";

}
TCPEndpoint::TCPEndpoint(const std::string& port) {
    inet_pton(AF_INET, HOST.c_str(), &(sa_.sin_addr));
    sa_.sin_family = AF_INET;
    std::istringstream is(port);
    std::uint16_t port_int{};
    is >> port_int;
    sa_.sin_port = htons(port_int);
}

sockaddr* TCPEndpoint::get() const {
    return (sockaddr*)&sa_;
}

const socklen_t TCPEndpoint::size() const {
    return sizeof(sa_);
}

const std::string TCPEndpoint::describe() const {
    std::ostringstream os;
    os << "TCP "<<inet_ntoa(sa_.sin_addr) << ":" << htons(sa_.sin_port);
    return os.str();
}

std::unique_ptr<IEndpoint> TCPEndpoint::create() const {
    return std::make_unique<TCPEndpoint>();
}

} // namespace remote_runnerd
