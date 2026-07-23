#include "utils.hpp"
#include <array>
#include <memory>
#include <cstdio>
#include <iostream>

CommandResult exec_cmd(const std::string& cmd) {
    std::array<char, 256> buffer;
    std::string result;
    std::string full_cmd = cmd + " 2>&1";
    
    FILE* pipe = popen(full_cmd.c_str(), "r");
    if (!pipe) {
        return {-1, "Failed to execute command pipe."};
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    
    int code = pclose(pipe);
    return {code == 0 ? 0 : WEXITSTATUS(code), result};
}

bool check_binary(const std::string& binary_name) {
    CommandResult res = exec_cmd("which " + binary_name);
    return res.exit_code == 0;
}
