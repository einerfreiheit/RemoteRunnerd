#pragma once
#include "Endpoint/Protocol.hpp"

#include <atomic>
#include <thread>
#include <memory>
#include <string>

namespace remote_runnerd {

class Configuration;
class Permissions;
class TaskRunner;
class IEndpoint;

class Server {
public:
    Server(const Configuration& configuration);
    void start();
    ~Server();

private:
    void handle(int fd);
    void init();

private:
    std::unique_ptr<TaskRunner> runner_;
    std::unique_ptr<Permissions> permissions_;
    std::unique_ptr<IEndpoint> endpoint_;
    size_t timeout_;
    int socket_;
    std::atomic_bool stop_flag_{false};
    std::thread signal_thread_;
};

} // namespace remote_runnerd