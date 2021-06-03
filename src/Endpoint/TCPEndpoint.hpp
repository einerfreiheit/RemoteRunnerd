#pragma once
#include "IEndpoint.hpp"

namespace remote_runnerd {

class TCPEndpoint : public IEndpoint {
public:
    TCPEndpoint() = default;
    TCPEndpoint(const std::string& port);

    sockaddr* get() const override;
    const size_t size() const override;
    std::string describe() const override;
    std::unique_ptr<IEndpoint> create() const override;

private:
    sockaddr_in sa_;
};

} // namespace remote_runnerd
