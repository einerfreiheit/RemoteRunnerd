#include "Communication/Session.hpp"
#include "RemoteTask.hpp"

#include <cstring>
#include <unistd.h>

namespace remote_runnerd {

void RemoteTask::execute(const std::vector<std::vector<char>>& commands,
                         size_t timeout,
                         const Session& session) {
    if (pipe(pipefd_) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    pid_t pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }

    if (pid == 0) {
        close(pipefd_[0]);
        alarm(timeout);
        std::vector<char*> params;
        params.reserve(commands.size());
        for (const auto& command : commands) {
            params.push_back(const_cast<char*>(command.data()));
        };
        params.push_back(nullptr);
        dup2(pipefd_[1], fileno(stdout));
        dup2(pipefd_[1], fileno(stderr));
        execvp(params[0], &params[0]);
    } else {
        close(pipefd_[1]);
        session.transmit(pipefd_[0]);
    }
}

} // namespace remote_runnerd
