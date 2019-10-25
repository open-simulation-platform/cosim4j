
#include <cse/python_model.hpp>
#include <cse/python_slave.hpp>

std::shared_ptr<const cse::model_description> python_model::description() const noexcept
{
    return std::shared_ptr<const cse::model_description>();
}
std::shared_ptr<cse::slave> python_model::instantiate_slave()
{
    return std::shared_ptr<cse::python_slave>();
}
std::shared_ptr<cse::async_slave> python_model::instantiate(std::string_view name)
{
    return cse::make_background_thread_slave(instantiate_slave());
}
