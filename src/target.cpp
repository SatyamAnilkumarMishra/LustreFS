
#include "target.hpp"
#include <iostream>
#include <cstdlib>

bool ISCSITarget::setup_target(const std::string& img_path, const std::string& target_iqn) {
    std::cout << "[+] Setting up iSCSI Backstore & Target via targetcli..." << std::endl;
    
    std::string cmd = "targetcli /backstores/fileio create storage_img " + img_path + " 1G write_back=false > /dev/null 2>&1;"
                      "targetcli /iscsi create " + target_iqn + " > /dev/null 2>&1;"
                      "targetcli /iscsi/" + target_iqn + "/tpg1/luns/ create /backstores/fileio/storage_img > /dev/null 2>&1;"
                      "targetcli /iscsi/" + target_iqn + "/tpg1/set attribute demo_mode_write_protect=0 generate_node_acls=1 cache_dynamic_acls=1 > /dev/null 2>&1";
    
    system(cmd.c_str());
    std::cout << "[+] Successfully exported " << img_path << " as iSCSI Target: " << target_iqn << std::endl;
    return true;
}
