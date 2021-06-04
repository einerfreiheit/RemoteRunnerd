#include "UDSEndpoint.hpp"

namespace remote_runnerd {

UDSEndpoint::UDSEndpoint(const std::string& path) {
    sa_.sun_family = AF_UNIX;
    strcpy(sa_.sun_path, path.c_str());
}

sockaddr* UDSEndpoint::get() const {
    return (sockaddr*)&sa_;
}
const socklen_t UDSEndpoint::size() const {
    return sizeof(sa_);
}
const std::string UDSEndpoint::describe() const {
    return std::string("UDS connection: ") + sa_.sun_path;
}
std::unique_ptr<IEndpoint> UDSEndpoint::create() const {
    return std::make_unique<UDSEndpoint>();
}

} // namespace remote_runnerd
