#include "RemoteTask.hpp"

#include "Communication/Session.hpp"
#include "Utils/TextProcessing.hpp"
#include "Utils/Utils.hpp"

#include <unistd.h>

namespace remote_runnerd {

void RemoteTask::execute(const std::vector<char>& commands,
                         size_t timeout,
                         const Session& session) {
    checkError(pipe(pipefd_), "Pipe has failed");
    pid_t pid = fork();
    checkError(pid, "Fork has failed");
    if (pid == 0) {
        close(pipefd_[0]);
        alarm(timeout);
        std::vector<char*> params;
        auto to_execute = split(commands);
        params.reserve(to_execute.size());
        for (const auto& command : to_execute) {
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
