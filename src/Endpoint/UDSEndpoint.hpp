#pragma once
#include "IEndpoint.hpp"
#include <sys/un.h>

namespace remote_runnerd {

class UDSEndpoint : public IEndpoint {
public:
    UDSEndpoint() = default;
    UDSEndpoint(const std::string& path);

    sockaddr* get() const override;
    const socklen_t size() const override;
    const std::string describe() const override;
    std::unique_ptr<IEndpoint> create() const override;

private:
    sockaddr_un sa_;
};

} // namespace remote_runnerd
