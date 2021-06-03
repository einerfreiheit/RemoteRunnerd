#include "EndpointFactory.hpp"
#include "TCPEndpoint.hpp"
#include "UDSEndpoint.hpp"

namespace remote_runnerd {

EndpointFactory::EndpointFactory() {
    mp_ = {{Protocol::TCP,
            [](const std::string& path) { return std::make_unique<TCPEndpoint>(path); }},
           {Protocol::UDS,
            [](const std::string& path) { return std::make_unique<UDSEndpoint>(path); }}};
}
std::unique_ptr<IEndpoint> EndpointFactory::make(Protocol protocol, const std::string& addr) const {
    return mp_.at(protocol)(addr);
}

} // namespace remote_runnerd
