#ifndef INITIATOR_HPP
#define INITIATOR_HPP

#include <string>

class ISCSIInitiator {
public:
    static bool connect_target(const std::string& target_ip, const std::string& target_iqn);
};

#endif 
