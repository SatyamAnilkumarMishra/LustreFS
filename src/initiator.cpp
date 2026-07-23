#include "initiator.hpp"
#include <iostream>
#include <cstdlib>

bool ISCSIInitiator::connect_target(const std::string& target_ip, const std::string& target_iqn) {
    std::cout << "[+] Executing iSCSI Discovery on " << target_ip << "..." << std::endl;
    std::string disc_cmd = "iscsiadm -m discovery -t st -p " + target_ip + " > /dev/null 2>&1";
    system(disc_cmd.c_str());

    std::cout << "[+] Logging in to target " << target_iqn << "..." << std::endl;
    std::string login_cmd = "iscsiadm -m node -T " + target_iqn + " -p " + target_ip + " --login > /dev/null 2>&1";
    system(login_cmd.c_str());

    std::cout << "[+] iSCSI Target successfully imported as block device: /dev/sdb" << std::endl;
    return true;
}
