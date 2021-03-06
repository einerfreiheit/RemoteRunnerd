#include "TaskRunner.hpp"
#include <iostream>

namespace remote_runnerd {
    
TaskRunner::TaskRunner() : stop_flag_(false) {
    unsigned int threads_num = std::thread::hardware_concurrency();
    threads_.reserve(threads_num);
    for (unsigned int i = 0; i < threads_num; ++i) {
        addThread();
    }
}

void TaskRunner::work() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->tasks_mtx_);
            cv_.wait(lock, [this] { return stop_flag_ || !tasks_.empty(); });
            if (stop_flag_) {
                return;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        try {
            task();
        } catch (const std::exception& e) {
            std::cerr << "Failed to execute task: " << e.what();
        }
    }
}

template <>
void TaskRunner::add<std::function, void>(std::function<void()>&& func) {
    {
        std::lock_guard<std::mutex> lock(this->tasks_mtx_);
        if (stop_flag_) {
            return;
        }
        tasks_.emplace(func);
    }
    cv_.notify_one();
}

void TaskRunner::addThread() {
    threads_.emplace_back([this] { this->work(); });
}

TaskRunner::~TaskRunner() {
    {
        std::lock_guard<std::mutex> lock(tasks_mtx_);
        stop_flag_ = true;
    }
    cv_.notify_all();
    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

} // namespace remote_runnerd
