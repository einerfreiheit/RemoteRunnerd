#include "Configuration.hpp"

#include <sstream>
#include <unistd.h>

namespace remote_runnerd {
    
namespace {
    
const std::string INSTRUCTIONS = "/etc/remote-runnerd.conf";

}

Configuration::Configuration(int argc, char** argv) {
    int token;
    while ((token = getopt(argc, argv, "a:p:t:")) != -1) {
        switch (token) {
        case 'a':{
            addr_ = optarg;
            break;
        }
        case 't': {
            std::istringstream is(optarg);
            is >> timeout_;
            break;
        }
        case 'p': {
            std::string protocol(optarg);
            if (protocol == "TCP") {
                protocol_ = Protocol::TCP;
            } else if (protocol == "UDS") {
                protocol_ = Protocol::UDS;
            } else {
                throw std::runtime_error("Unknown protocol: " + protocol);
            }
            break;
        }
        default:
            throw std::runtime_error(
                "usage: [-t timeout] [-p protocol: TCP or UDS] [-a address: port or path "
                "(depends on used protocol type)]");
        }
    }
}

const size_t Configuration::getTimeout() const {
    return timeout_;
}

const std::string Configuration::getAddress() const {
    return addr_;
}

const Protocol Configuration::getProptocol() const {
    return protocol_;
}

const std::string Configuration::getInstructions() const {
    return INSTRUCTIONS;
}

} // namespace remote_runnerd
