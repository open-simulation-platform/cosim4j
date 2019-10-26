
#ifndef CSECOREJNI_PYTHON_MODEL_HPP
#define CSECOREJNI_PYTHON_MODEL_HPP

#include <cse/orchestration.hpp>

#include <boost/filesystem.hpp>

#include <Python.h>

namespace cse {

class python_model: public cse::model {

public:
    explicit python_model(const boost::filesystem::path& py_file);

    std::shared_ptr<const cse::model_description> description() const noexcept override;

    std::shared_ptr<cse::slave> instantiate_slave();

    std::shared_ptr<cse::async_slave> instantiate(std::string_view name) override;

    ~python_model() override;

private:
    PyObject* pClass_;
    PyObject* pModule_;
    std::shared_ptr<const cse::model_description> modelDescription_;

};

}

#endif //CSECOREJNI_PYTHON_MODEL_HPP
