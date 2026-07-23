#ifndef ISCSI_TARGET_HPP
#define ISCSI_TARGET_HPP

#include <string>

class ISCSITarget {
public:
    static bool setup_export(const std::string& backstore_name, 
                             const std::string& img_path, 
                             const std::string& size_gb, 
                             const std::string& target_iqn);
    static bool teardown_export(const std::string& backstore_name, const std::string& target_iqn);
};

#endif
