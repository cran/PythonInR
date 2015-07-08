/* --------------------------------------------------------------------------  \

    CToR

    Functions to type cast C variables to R variables.

\  -------------------------------------------------------------------------- */

#ifndef C_TO_R_H
#define C_TO_R_H

#include "PythonInR.h"

SEXP c_to_r_boolean(int c_boolean);

SEXP c_to_r_integer(long c_long);

SEXP c_to_r_double(double c_double);

SEXP c_to_r_string(const char *c_string);

SEXP c_to_r_unicode(const char *c_unicode);

#endif
