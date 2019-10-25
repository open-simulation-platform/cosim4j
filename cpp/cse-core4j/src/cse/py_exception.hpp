
#ifndef CSECOREJNI_PY_EXCEPTION_HPP
#define CSECOREJNI_PY_EXCEPTION_HPP

#include <Python.h>
#include <exception>
#include <string>

namespace
{

inline void handle_py_exception()
{
    auto err = PyErr_Occurred();
    if (err != nullptr) {

        PyObject *pExcType, *pExcValue, *pExcTraceback;
        PyErr_Fetch(&pExcType, &pExcValue, &pExcTraceback);

        std::string error_msg = "An error occurred: ";
        if (pExcValue != nullptr) {
            PyObject* pRepr = PyObject_Repr(pExcValue);
            error_msg += PyUnicode_AsUTF8(pRepr);
            Py_DECREF(pRepr);
        } else {
            error_msg += "unknown error";
        }

        PyErr_Clear();

        Py_XDECREF(pExcType);
        Py_XDECREF(pExcValue);
        Py_XDECREF(pExcTraceback);

        throw std::runtime_error(error_msg);
    }
}

} // namespace

#endif
