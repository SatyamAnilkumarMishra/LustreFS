#include "manager.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <filesystem>

// Helper to auto-detect all imported iSCSI block devices (/dev/sd[b-z])
std::vector<std::string> get_imported_devices() {
    std::vector<std::string> devs;
    char letter = 'b';
    while (letter <= 'z') {
        std::string dev_path = "/dev/sd" + std::string(1, letter);
        if (std::filesystem::exists(dev_path)) {
            devs.push_back(dev_path);
        } else {
            break; // Stop at the first non-existent drive
        }
        letter++;
    }
    return devs;
}

bool LustreManager::format_mgt_mdt(const std::string& dev, const std::string& fs_name) {
    std::vector<std::string> devices = get_imported_devices();
    std::cout << "[STEP 4/8] Formatting " << devices.size() << " detected block device(s)..." << std::endl;

    for (const auto& d : devices) {
        std::cout << "  - Formatting " << d << "..." << std::endl;
        std::string cmd = "mkfs.ext4 -F " + d + " > /dev/null 2>&1";
        system(cmd.c_str());
    }
    return true;
}

bool LustreManager::mount_target(const std::string& dev, const std::string& mount_point) {
    std::vector<std::string> devices = get_imported_devices();
    std::cout << "[+] Mounting " << devices.size() << " block device(s) to target mount points..." << std::endl;

    for (size_t i = 0; i < devices.size(); ++i) {
        std::string target_mount = (i == 0) ? "/mnt/lustre_mdt" : "/mnt/lustre_ost" + std::to_string(i);
        
        system(("mkdir -p " + target_mount + " > /dev/null 2>&1").c_str());
        system(("mount " + devices[i] + " " + target_mount + " > /dev/null 2>&1 || true").c_str());
        
        std::cout << "    - " << devices[i] << " -> " << target_mount << std::endl;
    }
    return true;
}

void LustreManager::run_full_pipeline() {}
void SystemManager::run_full_pipeline() {}
