#pragma once

#include <string>
#include <vector>

namespace remote_runnerd {

class Session {
public:
    Session(int fd);
    Session& operator=(const Session& other) const = delete;
    Session(const Session& other) = delete;
    Session(Session&& other) = default;
    ~Session();

    std::vector<char> read() const;
    void write(std::string_view message) const;
    void transmit(int input) const;

private:
    void checkError(int return_code) const;

private:
    const int fd_;
    
};

} // namespace remote_runnerd
