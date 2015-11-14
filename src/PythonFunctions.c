#ifndef PYTHON_IN_R_FUNCTIONS
#define PYTHON_IN_R_FUNCTIONS
int ISO_C_forbids_an_empty_translation_unit;
#if ( defined(PY_EXPERIMENTAL) && defined(PYTHON_IN_R_NO_EXPLICIT_LINKING) )
#include "PythonFunctions.h"

/* --------------------------------------------------------------------------  \

    PythonFunction

    A collection of function which can be imported to Python.

\  -------------------------------------------------------------------------- */


/*  ----------------------------------------------------------------------------
    
    r_eval_py_string
     
    Is used to eval R code from Python.

  ----------------------------------------------------------------------------*/
PyObject *r_eval_py_string(PyObject* self, PyObject *args){
	PyObject *py_object;
    SEXP e, tmp, r_obj;
    int hadError;
    ParseStatus status;
    char* code = NULL;
    
    if (!PyArg_ParseTuple(args, "s", &code)) Py_RETURN_NONE;

    PROTECT(tmp = mkString(code));    
    PROTECT(e = R_ParseVector(tmp, 1, &status, R_NilValue));
    //PrintValue(e);
    r_obj = R_tryEval(VECTOR_ELT(e,0), R_GlobalEnv, &hadError);
    
    if (!hadError){
		py_object = r_to_py(r_obj);
		UNPROTECT(2);
		return(py_object);
	}else{
		Rprintf("an error occurred while executing the follwing expression\n");
		PrintValue(e);
		UNPROTECT(2);
		Py_RETURN_NONE;
	}
}

/*  ----------------------------------------------------------------------------
    
    py_get_r_object
     
    Get R objects from Python.

  ----------------------------------------------------------------------------*/
PyObject *py_get_r_object(PyObject* self, PyObject *py_name){
    SEXP r_obj;
    char *c_name = NULL;

    if (!PyArg_ParseTuple(py_name, "s", &c_name)) Py_RETURN_NONE;

    r_obj = findVar(install(c_name), R_GlobalEnv);

    if (r_obj == R_UnboundValue) {
        Rprintf("not found 1\n");
        return(PyLong_FromLong(-1));
    }

    PyObject *py_object = r_to_py(r_obj);
    return(py_object);
}

/*  ----------------------------------------------------------------------------
    
    py_set_r_object
     
	Set R objects from Python.

  ----------------------------------------------------------------------------*/
PyObject *py_set_r_object(PyObject *self, PyObject *args){
    PyObject *args_len, *args_i;
    SEXP key;
    char *c_key;

    args_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(args));
    long len = PY_TO_C_LONG(args_len);
    Py_XDECREF(args_len);

    if (len != 2) error("py_set_r_object wrong length!\n");

    args_i = PyLong_FromLong(0);
    c_key = PY_TO_C_STRING(PyTuple_GetItem(args, PyLong_AsSsize_t(args_i)));
    Py_XDECREF(args_i);

    args_i = PyLong_FromLong(1);
    // TODO: check if ref count is right
    SEXP val = py_to_r(PyTuple_GetItem(args, PyLong_AsSsize_t(args_i)), 1, 1);
    Py_XDECREF(args_i);

    key = Rf_install(c_key); // create a symbol
	setVar(key, val, R_GlobalEnv);
    
    return(PyLong_FromLong(0));
}

static PyMethodDef PythonInRMethods[] = {
    {"eval",  (PyCFunction)r_eval_py_string, METH_VARARGS, "comment"},
    {"get",  (PyCFunction)py_get_r_object, METH_VARARGS, "comment"},
    {"set",  (PyCFunction)py_set_r_object, METH_VARARGS, "comment"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC python_in_r_init_methods(void){
    (void) Py_InitModule("PythonInR", PythonInRMethods);
}

#endif
#endif
