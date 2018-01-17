#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP get_int_long_flag();
extern SEXP isDllVersion();
extern SEXP py_call_obj(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP py_close();
extern SEXP py_connect(SEXP);
extern SEXP py_get_info();
extern SEXP py_get_process_addresses();
extern SEXP py_import_append_logCatcher();
extern SEXP py_init_py_values();
extern SEXP py_init_redirect_stderrout();
extern SEXP py_initialize(SEXP);
extern SEXP py_is_connected();
extern SEXP py_run_simple_string(SEXP);
extern SEXP py_run_string_single_input(SEXP);
extern SEXP py_set_major_version(SEXP);
extern SEXP py_set_program_name(SEXP);
extern SEXP PythonInR_Run_String(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP r_set_py(SEXP, SEXP, SEXP);
extern SEXP r_set_py_dict(SEXP, SEXP, SEXP);
extern SEXP set_int_long_flag(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"get_int_long_flag",           (DL_FUNC) &get_int_long_flag,           0},
    {"isDllVersion",                (DL_FUNC) &isDllVersion,                0},
    {"py_call_obj",                 (DL_FUNC) &py_call_obj,                 5},
    {"py_close",                    (DL_FUNC) &py_close,                    0},
    {"py_connect",                  (DL_FUNC) &py_connect,                  1},
    {"py_get_info",                 (DL_FUNC) &py_get_info,                 0},
    {"py_get_process_addresses",    (DL_FUNC) &py_get_process_addresses,    0},
    {"py_import_append_logCatcher", (DL_FUNC) &py_import_append_logCatcher, 0},
    {"py_init_py_values",           (DL_FUNC) &py_init_py_values,           0},
    {"py_init_redirect_stderrout",  (DL_FUNC) &py_init_redirect_stderrout,  0},
    {"py_initialize",               (DL_FUNC) &py_initialize,               1},
    {"py_is_connected",             (DL_FUNC) &py_is_connected,             0},
    {"py_run_simple_string",        (DL_FUNC) &py_run_simple_string,        1},
    {"py_run_string_single_input",  (DL_FUNC) &py_run_string_single_input,  1},
    {"py_set_major_version",        (DL_FUNC) &py_set_major_version,        1},
    {"py_set_program_name",         (DL_FUNC) &py_set_program_name,         1},
    {"PythonInR_Run_String",        (DL_FUNC) &PythonInR_Run_String,        7},
    {"r_set_py",                    (DL_FUNC) &r_set_py,                    3},
    {"r_set_py_dict",               (DL_FUNC) &r_set_py_dict,               3},
    {"set_int_long_flag",           (DL_FUNC) &set_int_long_flag,           1},
    {NULL, NULL, 0}
};

void R_init_PythonInR(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
