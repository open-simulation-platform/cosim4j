
#include <cse/python_model.hpp>
#include <cse/python_slave.hpp>

namespace
{

std::shared_ptr<const cse::model_description> parse(const std::string& xml)
{
    auto desc = std::make_shared<cse::model_description>();

    return desc;
}

} // namespace

namespace cse
{

python_model::python_model(const boost::filesystem::path& py_file)
{
}

std::shared_ptr<const cse::model_description> python_model::description() const noexcept
{
    return modelDescription_;
}
std::shared_ptr<cse::slave> python_model::instantiate_slave()
{
    return std::shared_ptr<cse::python_slave>();
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
