/* --------------------------------------------------------------------------  \

    GetPyObjects

    Provides get functions for R.

\  -------------------------------------------------------------------------- */

#ifndef GET_PY_OBJECTS
#define GET_PY_OBJECTS

#include "PythonInR.h"

SEXP py_run_string_single_input(SEXP code);

SEXP get_py_obj(SEXP r_obj_name, SEXP simplify, SEXP auto_typecast);

PyObject *py_get_py_obj(const char *c_obj_name);

#endif
