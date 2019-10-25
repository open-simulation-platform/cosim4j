
#ifndef CSECOREJNI_PYTHON_MODEL_HPP
#define CSECOREJNI_PYTHON_MODEL_HPP

#include <cse/orchestration.hpp>

class python_model: public cse::model {

    std::shared_ptr<const cse::model_description> description() const noexcept override;

    std::shared_ptr<cse::slave> instantiate_slave();

    std::shared_ptr<cse::async_slave> instantiate(std::string_view name) override;

};

#endif //CSECOREJNI_PYTHON_MODEL_HPP
