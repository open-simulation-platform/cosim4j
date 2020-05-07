
#ifndef CSECOREJNI_CSE_EXECUTION_S_HPP
#define CSECOREJNI_CSE_EXECUTION_S_HPP

#include <cse.h>
#include <cse/execution.hpp>
#include <cse/algorithm.hpp>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct cse_execution_s
{
    std::unique_ptr<cse::execution> cpp_execution;
    cse::entity_index_maps entity_maps;

    size_t get_num_slaves()
    {
        return entity_maps.simulators.size();
    }

    cse::model_description get_model_description(cse_slave_index slave)
    {
        auto simulator = cpp_execution->get_simulator(slave);
        if (!simulator) {
            std::ostringstream oss;
            oss << "Slave with index " << slave << " was not found among loaded slaves.";
            throw std::invalid_argument(oss.str());
        }
        return simulator->model_description();
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
