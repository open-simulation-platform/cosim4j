
#include <cse/py_exception.hpp>
#include <cse/python_slave.hpp>

namespace cse
{

python_slave::python_slave(PyObject* pInstance, std::shared_ptr<const cse::model_description> modelDescription)
    : pInstance_(pInstance)
    , modelDescription_(std::move(modelDescription))
{
}

cse::model_description python_slave::model_description() const
{
    return *modelDescription_;
}

void python_slave::setup(cse::time_point startTime, std::optional<time_point> stopTime, std::optional<double> relativeTolerance)
{
    {
        auto f = PyObject_CallMethod(pInstance_, "setup_experiment", "(d)", startTime);
        if (f == nullptr) {
            handle_py_exception();
        }
        Py_DECREF(f);
    }
    {
        auto f = PyObject_CallMethod(pInstance_, "enter_initialization_mode", nullptr);
        if (f == nullptr) {
            handle_py_exception();
        }
        Py_DECREF(f);
    }
}

void python_slave::start_simulation()
{
    auto f = PyObject_CallMethod(pInstance_, "exit_initialization_mode", nullptr);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}
void python_slave::end_simulation()
{
    auto f = PyObject_CallMethod(pInstance_, "terminate", nullptr);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}

cse::step_result python_slave::do_step(cse::time_point currentT, cse::duration deltaT)
{
    double currentTime = cse::to_double_time_point(currentT);
    double stepSize = to_double_duration(deltaT, cse::to_time_point(0));
    auto f = PyObject_CallMethod(pInstance_, "do_step", "(dd)", currentTime, stepSize);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
    return step_result::complete;
}

void python_slave::get_real_variables(gsl::span<const value_reference> variables, gsl::span<double> values) const
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("d", 0.0));
    }

    auto f = PyObject_CallMethod(pInstance_, "__get_real__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);

    for (int i = 0; i < nvr; i++) {
        PyObject* value = PyList_GetItem(refs, i);
        values[i] = PyFloat_AsDouble(value);
    }

    Py_DECREF(refs);
}

void python_slave::get_integer_variables(gsl::span<const value_reference> variables, gsl::span<int> values) const
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(variables.size());
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("i", 0));
    }
    auto f = PyObject_CallMethod(pInstance_, "__get_integer__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);

    for (int i = 0; i < nvr; i++) {
        PyObject* value = PyList_GetItem(refs, i);
        values[i] = static_cast<int>(PyLong_AsLong(value));
    }

    Py_DECREF(refs);
}

void python_slave::get_boolean_variables(gsl::span<const value_reference> variables, gsl::span<bool> values) const
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("i", 0));
    }
    auto f = PyObject_CallMethod(pInstance_, "__get_boolean__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);

    for (int i = 0; i < nvr; i++) {
        PyObject* value = PyList_GetItem(refs, i);
        values[i] = PyObject_IsTrue(value);
    }

    Py_DECREF(refs);
}

void python_slave::get_string_variables(gsl::span<const value_reference> variables, gsl::span<std::string> values) const
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("s", ""));
    }
    auto f = PyObject_CallMethod(pInstance_, "__get_string__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);

    for (int i = 0; i < nvr; i++) {
        PyObject* value = PyList_GetItem(refs, i);
        values[i] = PyUnicode_AsUTF8(value);
    }

    Py_DECREF(refs);
}

void python_slave::set_real_variables(gsl::span<const value_reference> variables, gsl::span<const double> values)
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("d", values[i]));
    }

    auto f = PyObject_CallMethod(pInstance_, "__set_real__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    Py_DECREF(refs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}

void python_slave::set_integer_variables(gsl::span<const value_reference> variables, gsl::span<const int> values)
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("i", values[i]));
    }

    auto f = PyObject_CallMethod(pInstance_, "__set_integer__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    Py_DECREF(refs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}
void python_slave::set_boolean_variables(gsl::span<const value_reference> variables, gsl::span<const bool> values)
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, PyBool_FromLong(values[i]));
    }

    auto f = PyObject_CallMethod(pInstance_, "__set_boolean__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    Py_DECREF(refs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}
void python_slave::set_string_variables(gsl::span<const value_reference> variables, gsl::span<const std::string> values)
{
    if (variables.empty()) return;
    size_t nvr = variables.size();
    PyObject* vrs = PyList_New(nvr);
    PyObject* refs = PyList_New(nvr);
    for (int i = 0; i < nvr; i++) {
        PyList_SetItem(vrs, i, Py_BuildValue("i", variables[i]));
        PyList_SetItem(refs, i, Py_BuildValue("s", values[i].c_str()));
    }

    auto f = PyObject_CallMethod(pInstance_, "__set_string__", "(OO)", vrs, refs);
    Py_DECREF(vrs);
    Py_DECREF(refs);
    if (f == nullptr) {
        handle_py_exception();
    }
    Py_DECREF(f);
}

python_slave::~python_slave()
{
    Py_XDECREF(pInstance_);
}

}