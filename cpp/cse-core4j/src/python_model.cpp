
#include <cse/model_description_parser.hpp>
#include <cse/py_exception.hpp>
#include <cse/python_model.hpp>
#include <cse/python_slave.hpp>

#include <sstream>

namespace cse
{

python_model::python_model(const boost::filesystem::path& py_file)
{

    std::ostringstream oss;
    oss << "import sys\n";
    oss << "sys.path.append(r'" << py_file.parent_path().string() << "')\n";
    PyRun_SimpleString(oss.str().c_str());

    const auto moduleName = py_file.stem().string();
    pModule_ = PyImport_ImportModule(moduleName.c_str());
    if (pModule_ == nullptr) {
        handle_py_exception();
    }
    pClass_ = PyObject_GetAttrString(pModule_, "CseSlave");
    if (pClass_ == nullptr) {
        handle_py_exception();
    }
    PyObject* pInstance = PyObject_CallFunctionObjArgs(pClass_, nullptr);
    if (pInstance == nullptr) {
        handle_py_exception();
    }
    auto f = PyObject_CallMethod(pInstance, "define", nullptr);
    if (f == nullptr) {
        handle_py_exception();
    }
    const char* xml = PyUnicode_AsUTF8(f);
    modelDescription_ = parse_model_description(xml);

    Py_XDECREF(f);
    Py_XDECREF(pInstance);
}

std::shared_ptr<const cse::model_description> python_model::description() const noexcept
{
    return modelDescription_;
}
std::shared_ptr<cse::slave> python_model::instantiate_slave()
{
    PyObject* pInstance = PyObject_CallFunctionObjArgs(pClass_, nullptr);
    return std::make_shared<cse::python_slave>(pInstance, modelDescription_);
}
std::shared_ptr<cse::async_slave> python_model::instantiate(std::string_view name)
{
    return cse::make_background_thread_slave(instantiate_slave());
}

python_model::~python_model()
{
    Py_XDECREF(pClass_);
    Py_XDECREF(pModule_);
}

} // namespace cse
