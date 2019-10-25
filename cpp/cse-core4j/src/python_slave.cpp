
#include <cse/python_slave.hpp>

namespace cse {

cse::model_description python_slave::model_description() const
{
    return cse::model_description();
}
void python_slave::setup(cse::time_point startTime, std::optional<time_point> stopTime, std::optional<double> relativeTolerance)
{
}
void python_slave::start_simulation()
{
}
void python_slave::end_simulation()
{
}
cse::step_result python_slave::do_step(cse::time_point currentT, cse::duration deltaT)
{
    return step_result::complete;
}
void python_slave::get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const
{
}
void python_slave::get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const
{
}
void python_slave::get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const
{
}
void python_slave::get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const
{
}
void python_slave::set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values)
{
}
void python_slave::set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values)
{
}
void python_slave::set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values)
{
}
void python_slave::set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values)
{
}


}