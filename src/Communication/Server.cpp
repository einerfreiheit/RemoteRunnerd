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
#include <unistd.h>

namespace remote_runnerd {
namespace {

struct SignalContext {
    std::condition_variable cv;
    std::mutex mtx;
    bool state {false};
} ctx;

    
constexpr int MAX_CONNECTIONS = 100;

} // namespace

void signalHander(int) {
    std::unique_lock<std::mutex>(ctx.mtx);
    ctx.state = true;
    ctx.cv.notify_one();
}

Server::Server(const Configuration& configuration) : timeout_(configuration.getTimeout()) {
    permissions_ = std::make_unique<Permissions>(configuration.getInstructions());
    runner_ = std::make_unique<TaskRunner>();
    EndpointFactory factory;
    endpoint_ = factory.make(configuration.getProptocol(), configuration.getAddress());
    signal(SIGHUP, signalHander);
}

void Server::start() {
    permissions_->update();
    init();
    while (true) {
        socklen_t peer_addr_size;
        auto peer_endpoint = endpoint_->create();
        int acc = ::accept(socket_, peer_endpoint->get(), &peer_addr_size);
        std::string info = peer_endpoint->describe();
        std::cout << "New client: " << info << std::endl;
        try {
            handle(acc);
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to execute commands for " << info << std::endl;
        }
    }
}

void Server::init() {
    std::cout << "Starting on " << endpoint_->describe() << std::endl;
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
    
    signal_thread_ = std::thread([this]() {
        while (!stop_flag_) {
            std::unique_lock<std::mutex> lock(ctx.mtx);
            ctx.cv.wait(lock, [this] { return this->stop_flag_ || ctx.state; });
            if (ctx.state) {
                permissions_->update();
                ctx.state = false;
            }
        }
    });
}

void Server::handle(int fd) {
    if (fd < 0) {
        throw std::runtime_error(std::string("Failed to accept connection: ") +
                                 std::strerror(errno));
    }
    std::function<void()> func = [this, fd]() {
        Session session(fd);
        std::vector<char> message = session.read();
        auto commands = permissions_->split(message);
        if (!permissions_->isAllowed(commands)) {
            session.write("Failed to execute task: operation is not permitted\n");
            return;
        }
        RemoteTask task;
        task.execute(commands, timeout_, session);
    };
    runner_->add(func);
}

Server::~Server() {
    stop_flag_ = true;
    ctx.cv.notify_all();
    if (signal_thread_.joinable()) {
        signal_thread_.join();
    }
}
} // namespace remote_runnerd
