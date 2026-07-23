#ifndef IO_OPS_HPP
#define IO_OPS_HPP

#include <string>

class IOOps {
public:
    static bool write_direct_io(const std::string& filepath, const std::string& data);
    static bool evict_cache(const std::string& filepath);
    static std::string calculate_sha256(const std::string& filepath);
};

#endif
