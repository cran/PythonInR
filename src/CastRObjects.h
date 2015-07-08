/* --------------------------------------------------------------------------  \

    CastRObjects

    Provides functions cast R objects into Python objects!

\  -------------------------------------------------------------------------- */

#ifndef CAST_R_OBJECTS
#define CAST_R_OBJECTS

#include "PythonInR.h"

PyObject *r_logical_to_py_boolean(SEXP r_object);

PyObject *r_integer_to_py_long(SEXP r_object);

PyObject *r_numeric_to_py_double(SEXP r_object);

PyObject *r_character_to_py_string(SEXP r_object);

PyObject *r_character_to_py_unicode(SEXP r_object);

PyObject *r_to_py_primitive(SEXP r_object);

PyObject *r_to_py_tuple(SEXP r_object);

PyObject *r_to_py_list(SEXP r_object);

PyObject *r_to_py_dict(SEXP keys, SEXP value);

PyObject *r_to_py(SEXP r_object);

#endif
