/*#include "benchmark.hpp"
#include "utils.hpp"
#include <iostream>
#include <fstream>

bool BenchmarkRunner::run_fio_suite(const std::string& target_path, const std::string& output_dir) {
    std::cout << "[+] Generating dynamic fio test suite...\n";
    exec_cmd("mkdir -p " + output_dir);
    
    std::string config_path = output_dir + "/benchmark.fio";
    std::ofstream config(config_path);
    if (!config) return false;

    config << "[global]\n"
           << "ioengine=libaio\n"
           << "direct=1\n"
           << "group_reporting\n"
           << "size=64M\n"
           << "filename=" << target_path << "/fio_test_file\n\n"
           << "[seq_write]\n"
           << "rw=write\n"
           << "bs=1M\n\n"
           << "[rand_read]\n"
           << "rw=randread\n"
           << "bs=4k\n";
    config.close();

    std::cout << "[+] Running fio workload suite...\n";
    std::string run_cmd = "fio " + config_path + " --output=" + output_dir + "/fio_results.log";
    CommandResult res = exec_cmd(run_cmd);
    
    if (res.exit_code == 0) {
        std::cout << "[+] Benchmark complete. Log written to " << output_dir << "/fio_results.log\n";
        return true;
    }
    
    std::cerr << "[-] fio benchmark execution failed.\n";
    return false;
}
*/
#include "injector.hpp"    
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

bool FaultInjector::inject_iscsi_session_drop(const std::string& target_iqn) {
    std::cout << "[*] Simulating iSCSI session failure / network drop..." << std::endl;

    // Logout from target IQN session
    std::string logout_cmd = "iscsiadm -m node -T " + target_iqn + " --logout > /dev/null 2>&1";
    system(logout_cmd.c_str());

    std::cout << "[!] Session dropped. Waiting 3 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "[*] Attempting automatic target session recovery..." << std::endl;
    std::string login_cmd = "iscsiadm -m node -T " + target_iqn + " --login > /dev/null 2>&1";
    int res = system(login_cmd.c_str());

    if (res == 0) {
        std::cout << "[+] Session successfully recovered!" << std::endl;
        return true;
    } else {
        std::cerr << "[-] Failed to recover iSCSI session." << std::endl;
        return false;
    }
}
