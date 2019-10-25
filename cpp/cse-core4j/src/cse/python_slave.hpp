
#ifndef CSECOREJNI_PYTHON_SLAVE_HPP
#define CSECOREJNI_PYTHON_SLAVE_HPP

#include <cse/slave.hpp>

#include <Python.h>

namespace cse {

class python_slave: public cse::slave {

public:
    python_slave(PyObject* pInstance, std::shared_ptr<const cse::model_description> modelDescription);
    cse::model_description model_description() const override;
    void setup(cse::time_point startTime, std::optional<time_point> stopTime, std::optional<double> relativeTolerance) override;
    void start_simulation() override;
    void end_simulation() override;
    cse::step_result do_step(cse::time_point currentT, cse::duration deltaT) override;
    void get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const override;
    void get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const override;
    void get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const override;
    void get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const override;
    void set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values) override;
    void set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values) override;
    void set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values) override;
    void set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values) override;

    ~python_slave() override;

private:
    PyObject* pInstance_;
    std::shared_ptr<const cse::model_description> modelDescription_;

};

}

#endif //CSECOREJNI_PYTHON_SLAVE_HPP
