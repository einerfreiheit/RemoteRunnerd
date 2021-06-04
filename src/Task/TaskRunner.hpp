#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

namespace remote_runnerd {

class TaskRunner {
public:
    TaskRunner();
    TaskRunner& operator=(const TaskRunner& other) const = delete;
    TaskRunner(const TaskRunner& other) = delete;
    ~TaskRunner();
    template <template <class> class F, class R>
    void add(F<R()> func);

private:
    void addThread();
    void work();

private:
    std::queue<std::function<void()>> tasks_;
    bool stop_flag_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    std::mutex tasks_mtx_;
};

} // namespace remote_runnerd
