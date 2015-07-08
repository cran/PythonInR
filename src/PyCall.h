/* --------------------------------------------------------------------------  \

    PyCall

    Provides a function to call Python methods from R.

\  -------------------------------------------------------------------------- */

#include "PythonInR.h"
#include "CastRObjects.h"
#include "CastPyObjects.h"
#include "GetPyObjects.h"
#include "CToR.h"

SEXP py_call_obj(SEXP r_obj_name, SEXP r_args, SEXP r_kw, SEXP simplify, SEXP auto_typecast);
