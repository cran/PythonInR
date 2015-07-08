/* --------------------------------------------------------------------------  \

    CToR

    Provides some functions to transform C variables to R variables.

\  -------------------------------------------------------------------------- */
#include "CToR.h"
#include "PythonInR.h"

SEXP c_to_r_boolean(int c_boolean){
    SEXP r_val;
    PROTECT(r_val = allocVector(LGLSXP, 1));
    LOGICAL(r_val)[0] = c_boolean;
    UNPROTECT(1);
    return r_val;
}

SEXP c_to_r_integer(long c_long){
    SEXP r_val;
    PROTECT(r_val = allocVector(INTSXP, 1));
    INTEGER(r_val)[0] = c_long;
    UNPROTECT(1);
    return r_val;
}

SEXP c_to_r_double(double c_double){
    SEXP r_val;
    PROTECT(r_val = allocVector(REALSXP, 1));
    REAL(r_val)[0] = c_double;
    UNPROTECT(1);
    return r_val;
}

SEXP c_to_r_string(const char *c_string){
    SEXP r_val;
    PROTECT(r_val = allocVector(STRSXP, 1));
    SET_STRING_ELT(r_val, 0, mkChar(c_string));
    UNPROTECT(1);
    return r_val;
}

SEXP c_to_r_unicode(const char *c_unicode){
    SEXP r_val;
    PROTECT(r_val = allocVector(STRSXP, 1));
    SET_STRING_ELT(r_val, 0, mkCharCE(c_unicode, CE_UTF8));
    UNPROTECT(1);
    return r_val;
}
