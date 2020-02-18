
#ifndef CSECOREJNI_CSE_SLAVE_S_HPP
#define CSECOREJNI_CSE_SLAVE_S_HPP

#include <cse/slave.hpp>

#include <memory>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

struct cse_slave_s
{
    std::string address;
    std::string modelName;
    std::string instanceName;
    std::string source;
    std::shared_ptr<cse::slave> instance;
};

#ifdef __cplusplus
}
#endif

#endif //CSECOREJNI_CSE_SLAVE_S_HPP