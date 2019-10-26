
#ifndef CSECOREJNI_PY_STATE_HPP
#define CSECOREJNI_PY_STATE_HPP

#include <Python.h>

namespace cse {

class py_state
{
public:
    py_state()
    {
        Py_Initialize();
    }

    ~py_state()
    {
        Py_Finalize();
    }
};

}

#endif
