#include "manager.hpp"
#include <iostream>
#include <cstdlib>

bool LustreManager::format_mgt_mdt(const std::string& dev, const std::string& fs_name) {
    std::cout << "[STEP 4/8] Formatting Filesystem on " << dev << "..." << std::endl;
    int check_lustre = system("which mkfs.lustre > /dev/null 2>&1");
    if (check_lustre == 0) {
        std::cout << "[+] Formatting " << dev << " as Lustre MGT/MDT (" << fs_name << ")..." << std::endl;
        std::string cmd = "mkfs.lustre --reformat --fsname=" + fs_name + " --mgs --mdt " + dev + " > /dev/null 2>&1";
        system(cmd.c_str());
    } else {
        std::cout << "[!] mkfs.lustre not found natively. Formatting " << dev << " as ext4 fallback..." << std::endl;
        std::string cmd = "mkfs.ext4 -F " + dev + " > /dev/null 2>&1";
        system(cmd.c_str());
    }
    return true;
}

bool LustreManager::mount_target(const std::string& dev, const std::string& mount_point) {
    std::cout << "[+] Mounting " << dev << " at " << mount_point << "..." << std::endl;
    std::string mkdir_cmd = "mkdir -p " + mount_point;
    system(mkdir_cmd.c_str());
    
    std::string mount_cmd = "mount " + dev + " " + mount_point + " > /dev/null 2>&1 || true";
    system(mount_cmd.c_str());
    return true;
}

void LustreManager::run_full_pipeline() {}
void SystemManager::run_full_pipeline() {}
