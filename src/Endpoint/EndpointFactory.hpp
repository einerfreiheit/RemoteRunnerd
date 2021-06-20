#pragma once

#include "Protocol.hpp"

#include <functional>
#include <map>
#include <memory>

namespace remote_runnerd {

class IEndpoint;

class EndpointFactory {
public:
    EndpointFactory();
    std::unique_ptr<IEndpoint> make(Protocol protocol, const std::string& addr) const;

private:
    std::map<Protocol, std::function<std::unique_ptr<IEndpoint>(const std::string& addr)>> mp_;
};

} // namespace remote_runnerd
