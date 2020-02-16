#ifndef TASKRUNNER_H
#define TASKRUNNER_H

#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>
#include <unordered_map>

struct Task {
    int fd[2];
    std::string id;
    std::string output;
};
typedef std::shared_ptr<Task> TaskPtr;

class TaskRunner
{
public:
    void runTask(const std::vector<std::string>& cmd, const std::string& id);
    std::vector<TaskPtr> getResults();
    uint32_t timeout;
private:
    TaskPtr getResult(pid_t pid);
    std::unordered_map<pid_t, TaskPtr> tasks; 
    const int maxBufferSize = 4096;
};

#endif // TASKRUNNER_H
