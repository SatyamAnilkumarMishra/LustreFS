#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <string>

class BenchmarkRunner {
public:
    static bool run_fio_suite(const std::string& target_path, const std::string& output_dir);
};

#endif
