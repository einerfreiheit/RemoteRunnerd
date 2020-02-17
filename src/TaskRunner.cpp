#include "TaskRunner.h"
#include <stdexcept>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

std::vector<TaskPtr> TaskRunner::getResults()
{
    int status;
    pid_t child;
    std::vector<TaskPtr> result;
    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
        TaskPtr task = getResult(child);
        if (!task) continue;
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGALRM)
            task->output = "timeout expired\n";
        result.emplace_back(task);
    }
    return result;
}

TaskPtr TaskRunner::getResult(pid_t pid)
{
    const auto item = tasks.find(pid);
    if (item == tasks.end()) return TaskPtr();
    TaskPtr result = item->second;
    std::vector<int8_t> buffer(maxBufferSize);
    int nBytes = 0;
    do {
        nBytes = read(result->fd[0], &buffer[0], buffer.size());
        if (nBytes > 0) result->output.append(buffer.cbegin(), buffer.cbegin() + nBytes);
    } while (nBytes == maxBufferSize);
    close(result->fd[0]);
    tasks.erase(pid);
    return result;
}

void TaskRunner::runTask(const std::vector<std::string>& cmd, const std::string& id)
{
    if (cmd.empty()) return;
    TaskPtr task = std::make_shared<Task>();
    task->id = id;
    if (pipe(task->fd) < 0) {
        throw std::runtime_error(strerror(errno));
    }
    pid_t pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }

    if (pid == 0) {
        close(task->fd[0]);
        alarm(timeout);
        std::vector<char*> params;
        params.reserve(cmd.size());
        for (const std::string& str : cmd) {
            params.push_back(const_cast<char*>(str.c_str()));
        };
        params.push_back(nullptr);
        dup2(task->fd[1], fileno(stdout));
        dup2(task->fd[1], fileno(stderr));
        execvp(params[0], &params[0]);
        exit(0);
    } else {
        close(task->fd[1]);
        tasks.emplace(pid, task);
    }
}
