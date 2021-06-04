#include "Permissions.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

namespace remote_runnerd {
namespace {

constexpr char DELIMITER = ' ';

}

Permissions::Permissions(std::string path) : path_(std::move(path)) {}

std::vector<std::vector<char>> Permissions::split(const std::vector<char>& cmd) {
    std::vector<std::vector<char>> result;
    auto start = std::begin(cmd);
    auto end = start;
    while ((end = std::find(start, cmd.end(), DELIMITER)) != cmd.end()) {
        result.emplace_back(start, end++);
        start = end;
    }
    if (start != cmd.end()) {
        result.emplace_back(start, cmd.end());
    }
    return result;
}

void Permissions::update() {
    std::cout << "Reading permissions" << std::endl;
    std::ifstream input(path_);
    std::string token;
    if (!input.is_open()) {
        throw std::runtime_error(std::string("cannot open config file: " + path_));
    }
    {
        std::unique_lock<std::mutex> lock(mtx_);
        allowed_.clear();
        while (input >> token) {
            allowed_.emplace(token);
        }
    }
    input.close();
}

bool Permissions::isAllowed(std::vector<std::vector<char>>& splitted) const {
    if (splitted.empty()) {
        return false;
    }
    std::string name(splitted[0].begin(), splitted[0].end());
    std::unique_lock<std::mutex> lock(mtx_);

    return allowed_.find(name) != allowed_.end();
}

} // namespace remote_runnerd
