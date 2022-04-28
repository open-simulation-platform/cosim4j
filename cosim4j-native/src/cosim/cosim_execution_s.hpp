
#ifndef COSIMJNI_COSIM_EXECUTION_S_HPP
#define COSIMJNI_COSIM_EXECUTION_S_HPP

#include <cosim.h>
#include <cosim/execution.hpp>
#include <cosim/algorithm.hpp>

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

struct cosim_execution_s
{
    std::unique_ptr<cosim::execution> cpp_execution;
    cosim::entity_index_maps entity_maps;

    size_t get_num_slaves()
    {
        return entity_maps.simulators.size();
    }

    [[nodiscard]] cosim::model_description get_model_description(cosim_slave_index slave) const
    {
        return cpp_execution->get_model_description(slave);
    }

    [[nodiscard]] size_t get_num_variables(cosim_slave_index slave) const
    {
        auto description = get_model_description(slave);
        return description.variables.size();
    }
};

#ifdef __cplusplus
}
#endif

#endif //COSIMJNI_COSIM_EXECUTION_S_HPP
