#include "benchmark.hpp"
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
