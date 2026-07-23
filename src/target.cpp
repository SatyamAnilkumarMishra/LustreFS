#include "iscsi_target.hpp"
#include "utils.hpp"
#include <iostream>

bool ISCSITarget::setup_export(const std::string& backstore_name, 
                               const std::string& img_path, 
                               const std::string& size_gb, 
                               const std::string& target_iqn) {
    std::cout << "[+] Setting up iSCSI Backstore & Target via targetcli...\n";

    // 1. Create Backing File Store
    std::string cmd1 = "sudo targetcli /backstores/fileio create " + backstore_name + " " + img_path + " " + size_gb + "G";
    if (exec_cmd(cmd1).exit_code != 0) {
        std::cerr << "[-] Failed to create backstore fileio.\n";
        return false;
    }

    // 2. Create Target IQN
    std::string cmd2 = "sudo targetcli /iscsi create " + target_iqn;
    exec_cmd(cmd2); // Ignore if target already exists

    // 3. Create LUN
    std::string cmd3 = "sudo targetcli /iscsi/" + target_iqn + "/tpg1/luns/ create /backstores/fileio/" + backstore_name;
    if (exec_cmd(cmd3).exit_code != 0) {
        std::cerr << "[-] Failed to map LUN.\n";
        return false;
    }

    // 4. Configure ACL / Demo mode to permit access
    exec_cmd("sudo targetcli /iscsi/" + target_iqn + "/tpg1 set attribute demo_mode_write_protect=0");
    exec_cmd("sudo targetcli /iscsi/" + target_iqn + "/tpg1 set attribute generate_node_acls=1");
    exec_cmd("sudo targetcli /iscsi/" + target_iqn + "/tpg1 set attribute cache_dynamic_acls=1");

    std::cout << "[+] Successfully exported " << img_path << " as iSCSI Target: " << target_iqn << "\n";
    return true;
}

bool ISCSITarget::teardown_export(const std::string& backstore_name, const std::string& target_iqn) {
    std::cout << "[+] Tearing down iSCSI export...\n";
    exec_cmd("sudo targetcli /iscsi delete " + target_iqn);
    exec_cmd("sudo targetcli /backstores/fileio delete " + backstore_name);
    return true;
}
