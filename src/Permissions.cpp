#include "Permissions.hpp"

#include "Utils/TextProcessing.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace remote_runnerd {
namespace {

const std::vector<char> AND = {'&', '&'};

}

Permissions::Permissions(std::string path) : path_(std::move(path)) {}

void Permissions::update() {
    std::cout << "Reading permissions" << std::endl;
    std::ifstream input(path_);
    std::string token;
    if (!input.is_open()) {
        throw std::runtime_error(std::string("Cannot open configuration file: " + path_));
    }
    {
        std::lock_guard<std::mutex> lock(mtx_);
        allowed_.clear();
        while (input >> token) {
            allowed_.emplace(token);
        }
    }
    input.close();
}

bool Permissions::isAllowed(const std::vector<char>& commands) const {
    if (commands.empty()) {
        return false;
    }
    if (auto it = std::search(commands.begin(), commands.end(), AND.begin(), AND.end()); it != commands.end()) {
        return false;
    }
    std::string name = getCommandName(commands);
    std::lock_guard<std::mutex> lock(mtx_);
    return allowed_.find(name) != allowed_.end();
}

} // namespace remote_runnerd
