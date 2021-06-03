#pragma once
#include "Endpoint/Protocol.hpp"

#include <string>

namespace remote_runnerd {

class Configuration {
public:
    Configuration(int argc, char** argv);
    const size_t getTimeout() const;
    const std::string getAddress() const;
    const Protocol getProptocol() const;
    const std::string getInstructions() const;

private:
    size_t timeout_;
    std::string addr_;
    Protocol protocol_{Protocol::TCP};
};

} // namespace remote_runnerd
