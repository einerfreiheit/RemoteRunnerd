#pragma once
#include "IEndpoint.hpp"

namespace remote_runnerd {

class UDSEndpoint : public IEndpoint {
public:
    UDSEndpoint() = default;
    UDSEndpoint(const std::string& path);

    sockaddr* get() const override;
    const size_t size() const override;
    std::string describe() const override;
    std::unique_ptr<IEndpoint> create() const override;

private:
    sockaddr_un sa_;
};

} // namespace remote_runnerd
