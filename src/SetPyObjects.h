/* --------------------------------------------------------------------------  \

    SetPyObjects

    Provides set functions.

\  -------------------------------------------------------------------------- */

#ifndef SET_PY_OBJECTS
#define SET_PY_OBJECTS

#include "PythonInR.h"
#include "GetPyObjects.h"
#include "CastRObjects.h"
#include "CToR.h"

SEXP r_set_py(SEXP obj_name, SEXP key, SEXP value);
int py_set_py(const char *obj_name, const char *key, PyObject *value);

#endif
