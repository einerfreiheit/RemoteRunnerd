#include "InstructionProcessor.h"
#include <iterator>
#include <sstream>
#include <fstream>

InstructionProcessor::InstructionProcessor(const std::string& config) : config(config)
{
}

std::vector<std::string> InstructionProcessor::split(const std::string& cmd) const
{
    std::istringstream ss(cmd);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    return std::vector<std::string>(begin, end);
}

void InstructionProcessor::readPermissions()
{
    std::ifstream input(config);
    std::string token;
    if (!input.is_open()) {
        throw std::runtime_error(std::string("cannot open file: " + config));
    }
    while (input >> token)
        permitted.emplace(token);
    input.close();
}

bool InstructionProcessor::isCorrect(std::vector<std::string>& splitted) const
{
    return !splitted.empty() && (permitted.find(splitted[0]) != permitted.end());
}
