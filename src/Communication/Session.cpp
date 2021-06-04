#include "Session.hpp"

#include <cstring>
#include <stdexcept>
#include <unistd.h>

#include <iostream>

namespace remote_runnerd {
namespace {

constexpr char END = '\n';
    
}

Session::Session(int fd) : fd_(fd) {}

Session::~Session() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

std::vector<char> Session::read() const {
    std::vector<char> result;
    char buffer;
    int return_code = 0;
    while ((return_code = ::read(fd_, &buffer, 1)) > 0) {
        if (buffer == END) {
            break;
        }
        result.push_back(buffer);
    }
    checkError(return_code);
    return result;
}

void Session::write(std::string_view message) const {
    checkError(::write(fd_, message.data(), message.size()));
}

void Session::transmit(int input) const {
    char buffer;
    int return_code = 0;
    while ((return_code = ::read(input, &buffer, 1)) > 0) {
        checkError(::write(fd_, &buffer, 1));
    }
    checkError(return_code);
}

void Session::checkError(int return_code) const {
    if (return_code < 0) {
        throw std::runtime_error(std::strerror(errno));
    }
}

} // namespace remote_runnerd
