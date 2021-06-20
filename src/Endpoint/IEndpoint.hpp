#pragma once
#include <memory>
#include <string>
#include <sys/socket.h>

namespace remote_runnerd {

class IEndpoint {
public:
    virtual ~IEndpoint() = default;

    virtual sockaddr* get() const = 0;
    virtual const socklen_t size() const = 0;
    virtual const std::string describe() const = 0;
    virtual std::unique_ptr<IEndpoint> create() const = 0;
};

} // namespace remote_runnerd
