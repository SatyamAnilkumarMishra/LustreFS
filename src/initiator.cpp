#include "iscsi_initiator.hpp"
#include "utils.hpp"
#include <iostream>
#include <thread>
#include <chrono>

std::string ISCSIInitiator::discover_and_login(const std::string& target_ip, const std::string& target_iqn) {
    std::cout << "[+] Executing iSCSI Discovery on " << target_ip << "...\n";
    std::string disc_cmd = "sudo iscsiadm -m discovery -t sendtargets -p " + target_ip;
    if (exec_cmd(disc_cmd).exit_code != 0) {
        std::cerr << "[-] Discovery failed.\n";
        return "";
    }

    std::cout << "[+] Logging in to target " << target_iqn << "...\n";
    std::string login_cmd = "sudo iscsiadm -m node -T " + target_iqn + " -p " + target_ip + " --login";
    if (exec_cmd(login_cmd).exit_code != 0) {
        std::cerr << "[-] iSCSI login failed.\n";
        return "";
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Allow udev to settle

    // Discover mapped device name from /dev/disk/by-path/
    std::string dev_lookup = "ls -l /dev/disk/by-path/*" + target_iqn + "* | awk -F'/' '{print $NF}'";
    CommandResult dev_res = exec_cmd(dev_lookup);

    if (dev_res.exit_code == 0 && !dev_res.output.empty()) {
        std::string dev_name = dev_res.output;
        // Trim newline
        dev_name.erase(dev_name.find_last_not_of(" \n\r\t") + 1);
        std::string block_dev = "/dev/" + dev_name;
        std::cout << "[+] iSCSI Target successfully imported as block device: " << block_dev << "\n";
        return block_dev;
    }

    std::cerr << "[-] Mapped device path could not be resolved.\n";
    return "";
}

bool ISCSIInitiator::logout(const std::string& target_iqn) {
    std::cout << "[+] Logging out of iSCSI target " << target_iqn << "...\n";
    std::string logout_cmd = "sudo iscsiadm -m node -T " + target_iqn + " --logout";
    return exec_cmd(logout_cmd).exit_code == 0;
}
