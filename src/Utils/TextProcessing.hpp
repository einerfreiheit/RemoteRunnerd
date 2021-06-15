#pragma once

#include <string>
#include <vector>

namespace remote_runnerd {
    
std::vector<std::vector<char>> split(const std::vector<char>& cmd);
std::string getCommandName(const std::vector<char>& cmd);

} // namespace remote_runnerd
