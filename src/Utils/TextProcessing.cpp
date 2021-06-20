#include "TextProcessing.hpp"

#include <algorithm>

namespace remote_runnerd {
namespace {

constexpr char DELIMITER = ' ';

}

std::vector<std::vector<char>> split(const std::vector<char>& cmd) {
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

std::string getCommandName(const std::vector<char>& cmd) {
    auto end = std::find(cmd.begin(), cmd.end(), DELIMITER);
    return std::string(cmd.begin(), end);
}

} // namespace remote_runnerd
