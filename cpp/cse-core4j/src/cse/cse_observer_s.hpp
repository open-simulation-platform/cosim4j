
#ifndef CSECOREJNI_CSE_OBSERVER_S_HPP
#define CSECOREJNI_CSE_OBSERVER_S_HPP

#include <cse.h>
#include <cse/observer.hpp>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct cse_observer_s
{
    std::shared_ptr<cse::observer> cpp_observer;
};

#ifdef __cplusplus
}
#endif

#endif //CSECOREJNI_CSE_OBSERVER_S_HPP
