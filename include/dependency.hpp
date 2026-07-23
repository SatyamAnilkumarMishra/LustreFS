#ifndef DEPENDENCY_HPP
#define DEPENDENCY_HPP

class DependencyChecker {
public:
    static bool check_and_install();
    static bool audit_and_install();
};

class DependencyManager {
public:
    static bool check_and_install();
    static bool audit_and_install();
};

#endif // DEPENDENCY_HPP
