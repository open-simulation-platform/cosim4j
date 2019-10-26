
#ifndef CSECOREJNI_PYTHON_URI_SUB_RESOLVER_HPP
#define CSECOREJNI_PYTHON_URI_SUB_RESOLVER_HPP

#include <cse/orchestration.hpp>

namespace cse {

class python_uri_sub_resolver: public model_uri_sub_resolver {

public:
    std::shared_ptr<model> lookup_model(const uri& modelUri) override;
};

}

#endif
