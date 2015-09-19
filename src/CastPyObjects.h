/* --------------------------------------------------------------------------  \

    GetPyObjects

    Provides function to get Python objects from various name spaces.

\  -------------------------------------------------------------------------- */

#ifndef CAST_PY_OBJECTS
#define CAST_PY_OBJECTS

#include "PythonInR.h"

SEXP py_class(PyObject *py_object);

int Py_GetR_Type(PyObject *py_object);

int PyList_AllSameType(PyObject *py_object);

int PyTuple_AllSameType(PyObject *py_object);

int PyDict_AllSameType(PyObject *py_object);

SEXP py_dict_to_r_vec(PyObject *py_object, int r_vector_type);

SEXP py_list_to_r_vec(PyObject *py_object, int r_vector_type);

SEXP py_tuple_to_r_vec(PyObject *py_object, int r_vector_type);

SEXP py_list_to_r_list(PyObject *py_object, int simplify);

SEXP py_tuple_to_r_list(PyObject *py_object, int simplify);

SEXP py_dict_to_r_list(PyObject *py_object, int simplify);

int py_to_c_integer(PyObject *py_object);

const char *py_to_c_string(PyObject *py_object);

SEXP py_to_r(PyObject *py_object, int simplify, int autotype);

#endif
