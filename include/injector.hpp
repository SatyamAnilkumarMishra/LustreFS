#ifndef FAULT_INJECTOR_HPP
#define FAULT_INJECTOR_HPP

#include <string>

class FaultInjector {
public:
    static bool inject_iscsi_session_drop(const std::string& target_iqn);
    static bool inject_network_drop(const std::string& interface_name, int duration_sec);
    static bool simulate_target_crash();
};

#endif
