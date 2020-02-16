#ifndef INSTRUCTIONPROCESSOR_H
#define INSTRUCTIONPROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>

class InstructionProcessor
{
public:
    explicit InstructionProcessor(const std::string& config);
    ~InstructionProcessor() = default;
    void readPermissions();
    std::vector<std::string> split(const std::string& cmd) const;
    bool isCorrect(std::vector<std::string>& splitted) const;
private:
    std::unordered_set<std::string> permitted;
    std::string config;
};

#endif // INSTRUCTIONPROCESSOR_H
