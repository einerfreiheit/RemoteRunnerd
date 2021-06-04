#pragma once
#include "IEndpoint.hpp"

#include <netinet/in.h>

namespace remote_runnerd {

class TCPEndpoint : public IEndpoint {
public:
    TCPEndpoint() = default;
    TCPEndpoint(const std::string& port);

    sockaddr* get() const override;
    const socklen_t size() const override;
    const std::string describe() const override;
    std::unique_ptr<IEndpoint> create() const override;

private:
    sockaddr_in sa_;
};

} // namespace remote_runnerd
