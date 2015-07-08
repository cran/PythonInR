/* --------------------------------------------------------------------------  \

    PyRunString 

    PyRunString provides functions to run Pyhton code from within R.

\  -------------------------------------------------------------------------- */

#include "PyRunString.h"  

/*  ----------------------------------------------------------------------------
    py_run_simple_string 
    
    executes one line of Python code from R and prints the stdout at the screen.
    :param str code: The code that should be executed
    :return: NULL
    :rtype: NULL
  ----------------------------------------------------------------------------*/ 
SEXP py_run_string_single_input(SEXP code){
	PyObject *py_object, *py_main;
	int success = 0;
    const char *py_code = R_TO_C_STRING(code);
    py_main = PyModule_GetDict(PyImport_AddModule("__main__"));
    py_object = PyRun_String(py_code, Py_single_input, py_main, py_main);
    PyRun_SimpleString("\n");
    if (py_object == NULL) success = -1;
    Py_XDECREF(py_object);
    return c_to_r_integer(success);
}

/*  ----------------------------------------------------------------------------
    py_run_simple_string  
    
    (this could also be done with PythonInR_Run_String but since very
     small function I leave it)
    
    executes multiple lines of Python code from R.
    :param str code: The code that should be executed
    :return: Returns 0 on success or -1 if an exception was raised.
    :rtype: integer
  ----------------------------------------------------------------------------*/    
SEXP py_run_simple_string(SEXP code){
    const char *c_code = R_TO_C_STRING(code);
    int success = PyRun_SimpleString(c_code);
    PyRun_SimpleString("\n");
    return c_to_r_integer(success);
}

/*
 * PythonInR_Run_String
 * ====================
 * 
 * One Interface which uses the full capabilities of PyRun_String.
 * 
 *  PyObject* PyRun_String(const char *str, int start, PyObject *globals, PyObject *locals)
 *  Return value: New reference.
 * 
 *  The start parameter is the start symbol from the Python grammar to 
 *  use for interpreting the code. The possible start symbols are:
 *	 
 *	  Py_eval_input		for interpreting isolated expressions 		(#define Py_eval_input 258)
 *	  Py_file_input		for interpreting sequences of statements	(#define Py_file_input 257)
 *	  Py_single_input	for interpreting a single statement			(#define Py_single_input 256)
 * 
 * 
 * PythonInR_Run_String executes Python code and returns depending on 
 * return_to_r the thereby xreated variables to R.
 *
 *   :param str code: The code that should be executed.
 *   :param integer start_symbol: The start parameter (see above).
 *   :param boolean merge_namespaces: If true the local and the global 
 *                                    namespace are merged after execution.
 *   :param boolean override: If true new variables in the local namespace
 *                            override the existing in the global namespace.
 *   :param integer return_to_r: Possible values are:
 *                               0 nothing gets returned to R 
 *                               1 return the return value (py_object)
 *                               2 return the local variables
 *                               3 return the return value and local variables
 *                                 (testing showed that 3 actually never can 
 *                                  occur since PyRun_String returns NULL
 *                                  or does not allow to set values, therfore
 *                                  I did drop 3!)
 *   :param boolean simplify:
 *   :return: A list with all the variables created in the script.
 *   :rtype: list    
 *                                                                    */
SEXP PythonInR_Run_String(SEXP code, SEXP start_symbol, SEXP auto_typecast,
						  SEXP merge_namesp, SEXP override, SEXP return_to_r, 
						  SEXP simplify){
    SEXP r_locals, r_object;
    // all the python objects are defined here so one can easily mark and count them!
    PyObject *py_global, *py_local, *py_object, *py_len;
    const char *py_code;
    int success = 0;
    int c_merge_namesp = R_TO_C_BOOLEAN(merge_namesp);
    int c_override = R_TO_C_BOOLEAN(override);
    int c_simplify = R_TO_C_BOOLEAN(simplify);
    int autotype = R_TO_C_BOOLEAN(auto_typecast);
    long ret_to_r = R_TO_C_LONG(return_to_r);
    long vec_len;
    
    r_object = R_NilValue;
    py_code = R_TO_C_STRING(code);
	
    // PyImport_AddModule -> Borrowed reference                                 //            1    2    3    4
    // PyModule_GetDict -> Borrowed reference                                   // ---------------------------              
    py_global = PyModule_GetDict(PyImport_AddModule("__main__"));               // py_global  0 |  0 |  0 |  0 
    py_local = PyDict_New();                                                    // py_local  +1 | +1 | +1 | +1
    
    // PyRun_String -> New reference
    int c_start_symbol = R_TO_C_INT(start_symbol);
    py_object = PyRun_String(py_code, c_start_symbol, py_global, py_local);     // py_object  0 | +1 | +1 | +1
    
    if ( (py_object == NULL) | (ret_to_r < 2) ){ // cleanup py_local  (1)
        PyDict_Clear(py_local);
        Py_XDECREF(py_local);                                                   // py_local  -1 | -1 | -1 |  0
        if (py_object == NULL){                                                 // ===========================
			PyRun_SimpleString("\n");                                           // sum        0    1    1    2
			if (c_start_symbol == 258){
				error("\nKeyError: '%s'\n", py_code);
			}else{
				error("\n\toccured while executing:\n'%s'\n", py_code);
			}
		}else{
			if ( ret_to_r == 0 ){
				r_object = R_NilValue;
			}else{ // ret_to_r == 1
			  // TODO: add to py_to_r option new reference dir über grep gesetzt wird
                          //       und SEXP code (oder const char)
				r_object = py_to_r(py_object, c_simplify, autotype);
			}
			PyRun_SimpleString("\n");
			Py_XDECREF(py_object);
			return r_object;
		}
    }
   
    // Merge the namespaces 
    if (c_merge_namesp){
        // Return 0 on success or -1 if an exception was raised
        success = PyDict_Merge(py_global, py_local, c_override);
        if (success == -1){
            PyDict_Clear(py_local);
            Py_XDECREF(py_local);
            Py_XDECREF(py_object);
            PyRun_SimpleString("\n");
            error("the dictionaries could not be merged\n");
        }
    }
    
    py_len = PyLong_FromSsize_t(PyDict_Size(py_local));
	vec_len = PY_TO_C_LONG(py_len); Py_XDECREF(py_len);
	
    if ( ( py_local == NULL ) | ( vec_len < 1) ){
		PyRun_SimpleString("\n");
		Py_XDECREF(py_local);
		Py_XDECREF(py_object);
		return R_NilValue;
	}
	
	// Transform py_locals
	r_locals = py_to_r(py_local, c_simplify, autotype);
	// Cleanup
    Py_XDECREF(py_local);
    Py_XDECREF(py_object);
    PyRun_SimpleString("\n"); // just to flush the python error messages
	
	return r_locals;
}




