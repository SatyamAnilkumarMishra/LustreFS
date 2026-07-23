#include "target.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

bool ISCSITarget::setup_export(const std::string& backstore, const std::string& img_path, const std::string& size_gb, const std::string& target_iqn) {
    int num_disks = 1;
    
    // Interactive prompt for user input
    std::cout << "\n==================================================" << std::endl;
    std::cout << "[?] How many virtual disks would you like to create? ";
    if (!(std::cin >> num_disks) || num_disks < 1) {
        num_disks = 1; // Default fallback if invalid input
    }
    std::cout << "==================================================\n" << std::endl;

    std::cout << "[+] Creating " << num_disks << " backing disk image(s)..." << std::endl;

    // Create target IQN
    std::string create_iqn_cmd = "targetcli /iscsi create " + target_iqn + " > /dev/null 2>&1";
    system(create_iqn_cmd.c_str());

    // Loop to create N backstores and attach them as LUN 0, LUN 1, ..., LUN N-1
    for (int i = 1; i <= num_disks; ++i) {
        std::string store_name = "store_disk" + std::to_string(i);
        std::string file_path = "/tmp/iscsi_store" + std::to_string(i) + ".img";

        std::string cmd = 
            "targetcli /backstores/fileio create " + store_name + " " + file_path + " " + size_gb + "G write_back=false > /dev/null 2>&1;"
            "targetcli /iscsi/" + target_iqn + "/tpg1/luns/ create /backstores/fileio/" + store_name + " > /dev/null 2>&1;";
        system(cmd.c_str());
    }

    // Set ACL dynamic permissions
    std::string attr_cmd = "targetcli /iscsi/" + target_iqn + "/tpg1/set attribute demo_mode_write_protect=0 generate_node_acls=1 cache_dynamic_acls=1 > /dev/null 2>&1";
    system(attr_cmd.c_str());

    std::cout << "[+] Successfully exported " << num_disks << " disk image(s) under " << target_iqn << std::endl;
    return true;
}

bool ISCSITarget::setup_target(const std::string& img_path, const std::string& target_iqn) {
    return setup_export("fileio", img_path, "1", target_iqn);
}
