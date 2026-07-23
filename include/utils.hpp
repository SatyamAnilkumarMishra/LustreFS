#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

struct CommandResult {
    int exit_code;
    std::string output;
};

CommandResult exec_cmd(const std::string& cmd);
bool check_binary(const std::string& binary_name);

#endif
