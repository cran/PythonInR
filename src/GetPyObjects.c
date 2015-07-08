/* --------------------------------------------------------------------------  \

    GetPyObjects

    Provides function to get Python objects from various name spaces.

\  -------------------------------------------------------------------------- */

#include "GetPyObjects.h"
#include "PythonInR.h"
#include "CastPyObjects.h"

/*  --------------------------------------------------------------------
    get_py_obj (not used now but since it's only 7 lines of code I leave it for now)
        a simplified version which relies on the high level 
        function PyRun_String!
    ------------------------------------------------------------------*/
SEXP get_py_obj(SEXP r_obj_name, SEXP simplify, SEXP auto_typecast){
	SEXP r_object;
	
	const char *c_obj_name = R_TO_C_STRING(r_obj_name);
	
	// py_get_py_obj -> New reference
	PyObject *py_object = py_get_py_obj(c_obj_name);
	
	int c_simplify = R_TO_C_BOOLEAN(simplify);
	int autotype = R_TO_C_BOOLEAN(auto_typecast);
    r_object = py_to_r(py_object, c_simplify, autotype);
    Py_XDECREF(py_object);
    
    return r_object;
}	

/*  --------------------------------------------------------------------
    py_get_py_obj
        a simplified version which relies on the high level 
        function PyRun_String!
         
    Returns: New reference
	------------------------------------------------------------------*/
PyObject *py_get_py_obj(const char *c_obj_name){
    PyObject *py_global, *py_object;
	
	// get object (try a very high level approach)
	// PyImport_AddModule -> Borrowed reference
	// PyModule_GetDict -> Borrowed reference
    py_global = PyModule_GetDict(PyImport_AddModule("__main__"));
    
    // Py_eval_input, Py_file_input, or Py_single_input
    // PyRun_String -> New reference
    py_object = PyRun_String(c_obj_name, Py_eval_input, py_global, py_global);
    PyRun_SimpleString("\n");
    
    if (py_object == NULL){
        error("'%s' could not be found!", c_obj_name);
    }
    
    return py_object;
}
