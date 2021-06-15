#pragma once

#include <cstring>
#include <stdexcept>

namespace remote_runnerd {

inline void checkError(int return_code, const std::string& message) {
    if (return_code < 0) {
        throw std::runtime_error(message + ", reason: " + strerror(errno));
    }
}

} // namespace remote_runnerd
