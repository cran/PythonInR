/* --------------------------------------------------------------------------  \

    PyCall

    Provides functions call R objects from with in R!
    
    NOTE: PyCall should always return a new reference as it does now.
\  -------------------------------------------------------------------------- */

#include "PyCall.h"

SEXP py_call_obj(SEXP r_obj_name, SEXP r_args, SEXP r_kw, SEXP simplify, SEXP auto_typecast){
	SEXP r_object;
    PyObject *py_global, *py_object, *py_args, *py_kw, *py_ret_val;
	 
    // get object (try a very high level approach)
    const char *c_obj_name = R_TO_C_STRING(r_obj_name);
    int autotype = R_TO_C_BOOLEAN(auto_typecast);
    py_global = PyModule_GetDict(PyImport_AddModule("__main__"));
    
    // Py_eval_input, Py_file_input, or Py_single_input
    py_object = PyRun_String(c_obj_name, Py_eval_input, py_global, py_global);
    PyRun_SimpleString("\n");
    
    if (py_object == NULL){
        error("'%s' could not be found!", c_obj_name);
    }
    
    if (PyCallable_Check(py_object) < 1){
		error("'%s' is not callable!", c_obj_name);
    }
    
    // convert the args and kwargs
    // py_args = r_non_nested_list_to_py_tuple(r_args);
    long len_kw = GET_LENGTH(r_kw);
    py_args = r_to_py_tuple(r_args);
    if (len_kw < 1){
        py_kw = NULL;
    }else{
        py_kw = r_to_py_dict(GET_NAMES(r_kw), r_kw);
    }
    
    py_ret_val = PyObject_Call(py_object, py_args, py_kw);
    PyRun_SimpleString("\n");
    
    Py_XDECREF(py_args);
    Py_XDECREF(py_kw);
    Py_XDECREF(py_object);
    
    if (py_ret_val == NULL) error("error occured while calling '%s'", c_obj_name);
    
    int c_simplify = R_TO_C_BOOLEAN(simplify);
    r_object = py_to_r(py_ret_val, c_simplify, autotype);
    Py_XDECREF(py_ret_val);
    PyRun_SimpleString("\n");
    return r_object;
}


/* https://docs.python.org/2/c-api/object.html

int PyCallable_Check(PyObject *o)

    Determine if the object o is callable. Return 1 if the object is callable and 0 otherwise. This function always succeeds.

PyObject* PyObject_Call(PyObject *callable_object, PyObject *args, PyObject *kw)
    Return value: New reference.

    Call a callable Python object callable_object, with arguments given by the tuple args, and named arguments given by the dictionary kw. If no named arguments are needed, kw may be NULL. args must not be NULL, use an empty tuple if no arguments are needed. Returns the result of the call on success, or NULL on failure. This is the equivalent of the Python expression apply(callable_object, args, kw) or callable_object(*args, **kw).

    New in version 2.2.

PyObject* PyObject_CallObject(PyObject *callable_object, PyObject *args)
    Return value: New reference.

    Call a callable Python object callable_object, with arguments given by the tuple args. If no arguments are needed, then args may be NULL. Returns the result of the call on success, or NULL on failure. This is the equivalent of the Python expression apply(callable_object, args) or callable_object(*args).

PyObject* PyObject_CallFunction(PyObject *callable, char *format, ...)
    Return value: New reference.

    Call a callable Python object callable, with a variable number of C arguments. The C arguments are described using a Py_BuildValue() style format string. The format may be NULL, indicating that no arguments are provided. Returns the result of the call on success, or NULL on failure. This is the equivalent of the Python expression apply(callable, args) or callable(*args). Note that if you only pass PyObject * args, PyObject_CallFunctionObjArgs() is a faster alternative.

PyObject* PyObject_CallMethod(PyObject *o, char *method, char *format, ...)
    Return value: New reference.

    Call the method named method of object o with a variable number of C arguments. The C arguments are described by a Py_BuildValue() format string that should produce a tuple. The format may be NULL, indicating that no arguments are provided. Returns the result of the call on success, or NULL on failure. This is the equivalent of the Python expression o.method(args). Note that if you only pass PyObject * args, PyObject_CallMethodObjArgs() is a faster alternative.

PyObject* PyObject_CallFunctionObjArgs(PyObject *callable, ..., NULL)
    Return value: New reference.

    Call a callable Python object callable, with a variable number of PyObject* arguments. The arguments are provided as a variable number of parameters followed by NULL. Returns the result of the call on success, or NULL on failure.

    New in version 2.2.

PyObject* PyObject_CallMethodObjArgs(PyObject *o, PyObject *name, ..., NULL)
    Return value: New reference.

    Calls a method of the object o, where the name of the method is given as a Python string object in name. It is called with a variable number of PyObject* arguments. The arguments are provided as a variable number of parameters followed by NULL. Returns the result of the call on success, or NULL on failure.

    New in version 2.2.
*/
