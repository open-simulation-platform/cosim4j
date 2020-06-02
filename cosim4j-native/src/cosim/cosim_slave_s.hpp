
#ifndef COSIMJNI_COSIM_SLAVE_S_HPP
#define COSIMJNI_COSIM_SLAVE_S_HPP

#include <cosim/slave.hpp>

#include <memory>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

struct cosim_slave_s
{
    std::string address;
    std::string modelName;
    std::string instanceName;
    std::shared_ptr<cosim::slave> instance;
};

#ifdef __cplusplus
}
#endif

#endif //COSIMJNI_COSIM_SLAVE_S_HPP
