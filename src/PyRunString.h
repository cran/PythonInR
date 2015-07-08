/* --------------------------------------------------------------------------  \

   In this header some makros are defined to capture the API changes from 
   Python 2.7 to Python 3.

\  -------------------------------------------------------------------------- */

#ifndef PY_RUN_STRING
#define PY_RUN_STRING

#include "PythonInR.h"
#include "CastPyObjects.h"
#include "CToR.h"
#include "GetPyObjects.h"

SEXP py_run_simple_string(SEXP code);

SEXP PythonInR_Run_String(SEXP code, SEXP start_symbol, SEXP auto_typecast, 
						  SEXP merge_namesp, SEXP override, SEXP return_to_r, 
						  SEXP simplify);

#endif
