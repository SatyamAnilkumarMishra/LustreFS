#ifndef TARGET_HPP
#define TARGET_HPP

#include <string>

class ISCSITarget {
public:
    static bool setup_export(const std::string& backstore, const std::string& img_path, const std::string& size_gb, const std::string& target_iqn);
    static bool setup_target(const std::string& img_path, const std::string& target_iqn);
};

#endif // TARGET_HPP
