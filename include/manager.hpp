#ifndef LUSTRE_MANAGER_HPP
#define LUSTRE_MANAGER_HPP

#include <string>

class LustreManager {
public:
    static bool format_mgt_mdt(const std::string& dev, const std::string& fsname);
    static bool format_ost(const std::string& dev, const std::string& fsname, const std::string& mgs_node, int ost_index);
    static bool mount_target(const std::string& dev, const std::string& mount_point);
    static bool mount_client(const std::string& mgs_spec, const std::string& client_mount);
};

#endif
