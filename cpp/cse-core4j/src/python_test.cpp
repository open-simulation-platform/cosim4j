#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

namespace
{

}

int main(int argc, char* argv[])
{

    PyObject* pFunc;

    Py_Initialize();

    PyObject* pModule = PyImport_ImportModule("model");
    PyObject* modelClass = PyObject_GetAttrString(pModule, "Model");
    PyObject* modelInstance = PyObject_CallFunctionObjArgs(modelClass, nullptr);

    pFunc = PyObject_CallMethod(modelInstance, "setupExperiment", "(d)", 0.0);
    Py_XDECREF(pFunc);
    pFunc = PyObject_CallMethod(modelInstance, "enterInitializationMode", nullptr);
    Py_XDECREF(pFunc);
    pFunc = PyObject_CallMethod(modelInstance, "exitInitializationMode", nullptr);
    Py_XDECREF(pFunc);

    pFunc = PyObject_CallMethod(modelInstance, "doStep", "(dd)", 1.0, 0.1);
    Py_XDECREF(pFunc);

    Py_XDECREF(modelInstance);
    Py_XDECREF(modelClass);

    Py_DECREF(pModule);

    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}