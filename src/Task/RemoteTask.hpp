#pragma once

#include <memory>
#include <string>
#include <vector>

namespace remote_runnerd {

class Session;

class RemoteTask {
public:
    RemoteTask() = default;
    RemoteTask& operator=(const RemoteTask& other) const = delete;
    RemoteTask(const RemoteTask& other) = delete;
    void
    execute(const std::vector<std::vector<char>>& commands, size_t timeout, const Session& session);

private:
    int pipefd_[2];
};

} // namespace remote_runnerd
