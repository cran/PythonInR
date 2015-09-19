//#define DEBUG_PYTHONINR
/* --------------------------------------------------------------------------  \

    PythonInR

\  -------------------------------------------------------------------------- */

#ifndef PY_INSIDE_R_H
#define PY_INSIDE_R_H

extern long pyrNamespaceCounter;

#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING
#include <Python.h> // If you are a Linux user, did you realy install the Python headers? Try "sudo apt-get install python-dev" to fix this! :)
#else
#define STRICT_R_HEADERS // To disable some R code else I get an compile error
#endif

#include <string.h>
//#include <bytesobject.h>

#ifdef __unix__ /* __unix__ is usually defined by compilers targeting Unix systems */
    #include <dlfcn.h>
#elif defined _WIN32 /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */
    #include <windows.h>
#endif

#undef SIZEOF_SIZE_T 
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#ifdef DEBUG_PYTHONINR
#include <stdio.h>
#include <errno.h>
//static FILE * log_file;
#define logging(M, ...) fprintf(log_file, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); fflush(log_file); 
#else
#define logging(M, ...)
#endif

#ifndef PYTHON_IN_R_NO_EXPLICIT_LINKING
#include <winbase.h>
HMODULE py_hdll;
#include "Python_Explicit_Linking.h" // typedef all the functions needed!
#endif

#include "PythonInRMacros.h"
#include "GetPyObjects.h"
#include "CastPyObjects.h"
#include "CToR.h"


PyObject* log_write(PyObject*, PyObject*);
PyObject* log_flush(PyObject*, PyObject*);

#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING
SEXP py_connect(SEXP);
#else
SEXP py_connect(SEXP, SEXP, SEXP);

SEXP py_set_major_version(SEXP);

SEXP py_free_dll(void);

SEXP py_initialize(SEXP);

int py_get_api_version(void);

SEXP py_import_append_logCatcher(void);

SEXP py_init_redirect_stderrout(void);

SEXP py_init_py_values(void);

SEXP py_get_process_addresses(void);

SEXP py_set_program_name(SEXP);

SEXP py_set_home(SEXP pythonHome);

#endif

SEXP py_is_connected(void);

SEXP isDllVersion(void);

SEXP py_get_info(void);

SEXP py_close(void);

#endif
