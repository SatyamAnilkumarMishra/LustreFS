#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <string>

class LustreManager {
public:
    static bool format_mgt_mdt(const std::string& dev, const std::string& fs_name);
    static bool mount_target(const std::string& dev, const std::string& mount_point);
    static void run_full_pipeline();
};

class SystemManager {
public:
    static void run_full_pipeline();
};

#endif
