#ifndef DEPENDENCY_HPP
#define DEPENDENCY_HPP

#include <vector>
#include <string>

class DependencyManager {
public:
    static bool audit_and_install();
private:
    static const std::vector<std::string> required_tools;
};

#endif
