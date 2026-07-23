#ifndef TARGET_HPP
#define TARGET_HPP

#include <string>

class ISCSITarget {
public:
    static bool setup_target(const std::string& img_path, const std::string& target_iqn);
};

#endif 
