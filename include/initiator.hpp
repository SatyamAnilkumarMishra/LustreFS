#ifndef ISCSI_INITIATOR_HPP
#define ISCSI_INITIATOR_HPP

#include <string>

class ISCSIInitiator {
public:
    static std::string discover_and_login(const std::string& target_ip, const std::string& target_iqn);
    static bool logout(const std::string& target_iqn);
};

#endif
