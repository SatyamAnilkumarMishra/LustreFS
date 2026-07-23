#ifndef INJECTOR_HPP
#define INJECTOR_HPP

#include <string>

class FaultInjector {
public:
    static bool inject_iscsi_session_drop(const std::string& target_iqn);
};

#endif 
