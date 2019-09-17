
#ifndef CSECOREJNI_CSE_EXECUTION_S_HPP
#define CSECOREJNI_CSE_EXECUTION_S_HPP

#include <cse.h>
#include <cse/execution.hpp>
#include <cse/ssp_parser.hpp>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct cse_execution_s
{
    cse::simulator_map simulators;
    std::unique_ptr<cse::execution> cpp_execution;

    size_t get_num_slaves()
    {
        return simulators.size();
    }

    cse::model_description get_model_description(cse_slave_index slave)
    {
        for (const auto& entry : simulators) {
            if (entry.second.index == slave) {
                return entry.second.description;
            }
        }
        std::ostringstream oss;
        oss << "Slave with index " << slave << " was not found among loaded slaves.";
        throw std::invalid_argument(oss.str());
    }

    size_t get_num_variables(cse_slave_index slave)
    {
        auto description = get_model_description(slave);
        return description.variables.size();
    }
};

#ifdef __cplusplus
}
#endif

#endif //CSECOREJNI_CSE_EXECUTION_S_HPP
