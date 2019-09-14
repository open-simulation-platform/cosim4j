
#ifndef CSECOREJNI_STRUCTS_HPP
#define CSECOREJNI_STRUCTS_HPP

#include <cse.h>
#include <cse/execution.hpp>
#include <cse/model.hpp>
#include <cse/ssp_parser.hpp>

struct cse_observer_s
{
    std::shared_ptr<cse::observer> cpp_observer;
};

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

#endif
