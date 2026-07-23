#include "dependency.hpp"
#include "utils.hpp"
#include <iostream>

const std::vector<std::string> DependencyManager::required_tools = {
    "targetcli", "iscsiadm", "mkfs.lustre", "fio"
};

bool DependencyManager::audit_and_install() {
    std::cout << "[+] Auditing system dependencies...\n";
    bool all_ok = true;

    for (const auto& tool : required_tools) {
        if (!check_binary(tool)) {
            std::cout << "[-] Missing dependency: " << tool << ". Attempting automatic installation...\n";
            CommandResult res = exec_cmd("sudo apt-get update && sudo apt-get install -y " + tool + " || sudo dnf install -y " + tool);
            if (res.exit_code != 0) {
                std::cerr << "[!] Failed to install " << tool << ": " << res.output << "\n";
                all_ok = false;
            } else {
                std::cout << "[+] Successfully installed " << tool << "\n";
            }
        } else {
            std::cout << "  [✓] Found " << tool << "\n";
        }
    }
    return all_ok;
}
