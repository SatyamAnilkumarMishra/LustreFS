#include "lustre_manager.hpp"
#include "utils.hpp"
#include <iostream>

bool LustreManager::format_mgt_mdt(const std::string& dev, const std::string& fsname) {
    std::cout << "[+] Formatting " << dev << " as combined MGT/MDT...\n";
    std::string cmd = "sudo mkfs.lustre --fsname=" + fsname + " --mgs --mdt --reformat " + dev;
    CommandResult res = exec_cmd(cmd);
    if (res.exit_code != 0) {
        std::cerr << "[-] Failed mkfs.lustre (MDT): " << res.output << "\n";
        return false;
    }
    return true;
}

bool LustreManager::format_ost(const std::string& dev, const std::string& fsname, const std::string& mgs_node, int ost_index) {
    std::cout << "[+] Formatting " << dev << " as OST " << ost_index << "...\n";
    std::string cmd = "sudo mkfs.lustre --ost --fsname=" + fsname + " --mgsnode=" + mgs_node + " --index=" + std::to_string(ost_index) + " --reformat " + dev;
    CommandResult res = exec_cmd(cmd);
    if (res.exit_code != 0) {
        std::cerr << "[-] Failed mkfs.lustre (OST): " << res.output << "\n";
        return false;
    }
    return true;
}

bool LustreManager::mount_target(const std::string& dev, const std::string& mount_point) {
    exec_cmd("sudo mkdir -p " + mount_point);
    std::cout << "[+] Mounting target " << dev << " at " << mount_point << "...\n";
    std::string cmd = "sudo mount -t lustre " + dev + " " + mount_point;
    return exec_cmd(cmd).exit_code == 0;
}

bool LustreManager::mount_client(const std::string& mgs_spec, const std::string& client_mount) {
    exec_cmd("sudo mkdir -p " + client_mount);
    std::cout << "[+] Mounting Lustre client on " << client_mount << "...\n";
    std::string cmd = "sudo mount -t lustre " + mgs_spec + " " + client_mount;
    return exec_cmd(cmd).exit_code == 0;
}
