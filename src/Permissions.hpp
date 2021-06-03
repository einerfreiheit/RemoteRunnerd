#pragma once

#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

namespace remote_runnerd {

class Permissions {
public:
    explicit Permissions(std::string path);
    void update();
    static std::vector<std::vector<char>> split(const std::vector<char>& cmd);
    bool isAllowed(std::vector<std::vector<char>>& splitted) const;

private:
    std::unordered_set<std::string> allowed_;
    std::string path_;
    mutable std::mutex mtx_;
};

} // namespace remote_runnerd
