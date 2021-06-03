#include "Configuration.hpp"
#include "Endpoint/EndpointFactory.hpp"
#include "Endpoint/IEndpoint.hpp"
#include "Permissions.hpp"
#include "Server.hpp"
#include "Session.hpp"
#include "Task/RemoteTask.hpp"
#include "Task/TaskRunner.hpp"

#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

namespace remote_runnerd {
namespace {
    
constexpr int MAX_CONNECTIONS = 100;
volatile std::sig_atomic_t signal_state;

} // namespace

void signalHander(int signal) {
    signal_state = signal;
}

Server::Server(const Configuration& configuration) : timeout_(configuration.getTimeout()) {
    permissions_ = std::make_unique<Permissions>(configuration.getInstructions());
    runner_ = std::make_unique<TaskRunner>();
    EndpointFactory factory;
    endpoint_ = factory.make(configuration.getProptocol(), configuration.getAddress());
    signal(SIGHUP, signalHander);
}

void Server::start() {
    init();
    while (true) {
        socklen_t peer_addr_size;
        auto peer_endpoint = endpoint_->create();
        int acc = accept(socket_, peer_endpoint->get(), &peer_addr_size);
        std::string info = peer_endpoint->describe();
        std::cout << "Got new client: " << info << std::endl;
        try {
            handle(acc);
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to execute commands for " << info << std::endl;
        }
        std::cout << "Request from " << info << " completed" << std::endl;
    }
}

void Server::init() {
    socket_ = socket(endpoint_->get()->sa_family, SOCK_STREAM, 0);
    if (socket_ < 0) {
        throw std::runtime_error((std::string("Failed to create socket ") + std::strerror(errno)));
    }
    if (bind(socket_, endpoint_->get(), endpoint_->size()) < 0) {
        throw std::runtime_error(std::string("Failed to bind socket ") + std::strerror(errno));
    }
    if (listen(socket_, MAX_CONNECTIONS) < 0) {
        throw std::runtime_error(std::string("Failed to start to listen ") + std::strerror(errno));
    }
}

void Server::handle(int fd) {
    if (fd < 0) {
        if (errno == EINTR && signal_state) {
            permissions_->update();
            signal_state = 0;
            return;
        } else {
            throw std::runtime_error(std::string("Failed to accept connection: ") +
                                     std::strerror(errno));
        }
    }
    auto handle = [this, fd]() {
        Session session(fd);
        std::vector<char> message = session.read();
        auto commands = permissions_->split(message);
        if (!permissions_->isAllowed(commands)) {
            session.write("Failed to execute task: operation is not permitted");
            return;
        }
        RemoteTask task;
        task.execute(commands, timeout_, session);
    };
    runner_->add(handle);
}

Server::~Server() = default;

} // namespace remote_runnerd
