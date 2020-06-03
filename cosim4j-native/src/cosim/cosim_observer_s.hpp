
#ifndef COSIMJNI_COSIM_OBSERVER_S_HPP
#define COSIMJNI_COSIM_OBSERVER_S_HPP

#include <cosim.h>
#include <cosim/observer.hpp>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct cosim_observer_s
{
    std::shared_ptr<cosim::observer> cpp_observer;
};

#ifdef __cplusplus
}
#endif

#endif //COSIMJNI_COSIM_OBSERVER_S_HPP
