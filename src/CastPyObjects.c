/*  ----------------------------------------------------------------------------

    PyToRbasic

    Provides functions to convert non nested Python data types to R data types.

  ----------------------------------------------------------------------------*/

#include "CastPyObjects.h"
#include "PythonInR.h"
#include "CToR.h"

//#define     NILSXP       0 /* nil = NULL */
//#define     SYMSXP       1 /* symbols */
//#define     LISTSXP      2 /* lists of dotted pairs */
//#define     CLOSXP       3 /* closures */
//#define     ENVSXP       4 /* environments */
//#define     PROMSXP      5 /* promises: [un]evaluated closure arguments */
//#define     LANGSXP      6 /* language constructs (special lists) */
//#define     SPECIALSXP   7 /* special forms */
//#define     BUILTINSXP   8 /* builtin non-special forms */
//#define     CHARSXP      9 /* "scalar" string type (internal only)*/
//#define     LGLSXP      10 /* logical vectors */
//#define     INTSXP      13 /* integer vectors */
//#define     REALSXP     14 /* real variables */
//#define     CPLXSXP     15 /* complex variables */
//#define     STRSXP      16 /* string vectors */
//#define     DOTSXP      17 /* dot-dot-dot object */

/*  ----------------------------------------------------------------------------

    py_class 
      transforms python into class objects
      
      Increases the reference count from py_object +1
      
      TODO: The reference counts are consistent with PythonInR_Run_String, 
            double check if it is also consistient with the other use cases!
      
      TODO: Allow the user to overload this meachnism, so I can control
            from R if a PythonObject should be returned or another thing.

  ----------------------------------------------------------------------------*/
SEXP py_class(PyObject *py_object){
	SEXP r_list, r_list_names;
	PyObject *py_main, *py_pyinr, *py_r_namesp;
    
    //Rprintf("pyrNamespaceCounter: %i\n", pyrNamespaceCounter);
    
    // Since the value can't be returned to R I save it in Python
    // PyModule_GetDict -> Borrowed reference, PyImport_AddModule -> Borrowed reference
    py_main = PyModule_GetDict(PyImport_AddModule("__main__"));
    // PyDict_GetItemString -> Borrowed reference
    py_pyinr = PyDict_GetItemString(py_main, "__R__"); // PyModule_GetDict(PyImport_AddModule("__R__")); //   
    if (py_pyinr == NULL) error("Couldn't find the object '__R__'\n");
    
    py_r_namesp = PyObject_GetAttrString(py_pyinr, "namespace"); // 	PyObject_GetItem(py_pyinr, py_key)
    if (py_r_namesp == NULL) error("Couldn't find the object 'namespace'\n");
    int success = PyDict_SetItem(py_r_namesp, PyLong_FromLong(pyrNamespaceCounter), py_object);
    if (success < 0) Rprintf("Error int py_class PyDict_SetItem returned -1\n"); // TODO: make real error handling!
    PROTECT(r_list = allocVector(VECSXP, 3));
    PROTECT(r_list_names = allocVector(VECSXP, 3));
    
    // set isCallable
    int c_is_callable = PyCallable_Check(py_object);
    SET_VECTOR_ELT(r_list_names, 0, c_to_r_string("isCallable"));
    SET_VECTOR_ELT(r_list, 0, c_to_r_integer(c_is_callable));
    
    // set id
    SET_VECTOR_ELT(r_list_names, 1, c_to_r_string("id"));
    SET_VECTOR_ELT(r_list, 1, c_to_r_integer(pyrNamespaceCounter));
    
    // class name   
    // This is taken from the python soure, they normaly use the Makro
    // Py_TYPE e.g.
    // #define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
    // Py_TYPE(arg)->tp_name
    // since I only use it once I just use it directly since they also
    // use it in error messages it should never fail!
    // const char *c_type_name = (((PyObject*)(py_object))->ob_type)->tp_name;
    const char *c_type_name = Py_TYPE(py_object)->tp_name;
	SET_VECTOR_ELT(r_list_names, 2, c_to_r_string("type"));
    SET_VECTOR_ELT(r_list, 2, c_to_r_string(c_type_name));
    
    setAttrib(r_list, R_NamesSymbol, r_list_names);
    	
	// Every Value which can't be transformed is returned to R as PythonObject
	classgets(r_list, c_to_r_string("PythonObject"));
	
	UNPROTECT(2);
    
    pyrNamespaceCounter = pyrNamespaceCounter + 1;
	
    return r_list;
}

/*  ----------------------------------------------------------------------------

    Py_GetR_Type 
      returns the integer which is needed for R vector allocation

  ----------------------------------------------------------------------------*/
int Py_GetR_Type(PyObject *py_object){
    int r_type = -1;

    if ( PyNone_Check(py_object) ){
        r_type = 0;
    }else if ( PyBool_Check(py_object) ){
        r_type = 10;
    }else if ( PyInt_Check(py_object) ){
        r_type = 13;
    }else if ( PyLong_Check(py_object) ){
        r_type = 13;
    }else if ( PyFloat_Check(py_object) ){
        r_type = 14;
    }else if ( PyString_Check(py_object) ){
        r_type = 16;
    }else if ( PyUnicode_Check(py_object) ){
        r_type = 16;
    }
    return r_type;
}

/*  ----------------------------------------------------------------------------
    PyList_AllSameType
    --------------------------------------------------------------------------*/
int PyList_AllSameType(PyObject *py_object){
    PyObject *item, *py_len, *py_i;
    long list_len, count;
    int r_type = -1, item_type;

    py_len = PyLong_FromSsize_t(PyList_GET_SIZE(py_object));
    list_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);
    
    py_i = PyLong_FromLong(0);
    item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
    Py_XINCREF(item); // If item is NULL Py_XINCREF has no effect.
    Py_XDECREF(py_i);
    if ( item == NULL ) return 0;
    r_type = Py_GetR_Type(item); // just makes sence for the types which exists in R
    Py_XDECREF(item);
    if ( r_type == -1 ) return r_type; // -1 is returned if it is not (None, boolean, int, long, float, string or unicode)

    count = 0;
    for (long i=1; i < list_len; i++){
        py_i = PyLong_FromLong(i);
        item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        Py_XDECREF(py_i);
        item_type = Py_GetR_Type(item);
        count += ( (r_type != item_type) && (0 != item_type) );
        Py_XDECREF(item);
        if (count > 0) break;
    }

    r_type = (count == 0) ? r_type : -1;

    return r_type;
}

/*  ----------------------------------------------------------------------------
    PyTuple_AllSameType
    --------------------------------------------------------------------------*/
int PyTuple_AllSameType(PyObject *py_object){
    PyObject *item, *py_len, *py_i;
    long list_len, count;
    int r_type = -1, item_type;

    py_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(py_object));
    list_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);

    py_i = PyLong_FromLong(0);
    item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
    Py_XINCREF(item);
    Py_XDECREF(py_i);
    if ( item == NULL ) return 0;
    r_type = Py_GetR_Type(item); // just makes sence for the types which exists in R
    Py_DECREF(item);
    if ( r_type == -1 ) return r_type;

    count = 0;
    for (long i=1; i < list_len; i++){
        py_i = PyLong_FromLong(i);
        item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        Py_XDECREF(py_i);
        item_type = Py_GetR_Type(item);
        count += ( (r_type != item_type) && (0 != item_type) );
        Py_XDECREF(item);
        if (count > 0) break;
    }

    r_type = (count == 0) ? r_type : -1;

    return r_type;
}

/*  ----------------------------------------------------------------------------
    PyDict_AllSameType
    --------------------------------------------------------------------------*/
int PyDict_AllSameType(PyObject *py_object){
    PyObject *py_values;
    int r_type;
    // PyDict_Values -> New reference
    py_values = PyDict_Values(py_object);
    r_type = PyList_AllSameType(py_values);
    Py_XDECREF(py_values);
    return  r_type;
}

/*  ----------------------------------------------------------------------------

    py_dict_to_r_vec

  ----------------------------------------------------------------------------*/
SEXP py_dict_to_r_vec(PyObject *py_object, int r_vector_type){
    PyObject *item, *py_keys, *py_values, *py_len, *py_i;
    SEXP r_vec, r_vec_names;
    long vec_len;

	if (r_vector_type == 0) return R_NilValue;

    py_len = PyLong_FromSsize_t(PyDict_Size(py_object));
    vec_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);
    item = NULL;
    py_keys = PyDict_Keys(py_object);
    py_values = PyDict_Values(py_object);

    PROTECT(r_vec = allocVector(r_vector_type, vec_len));
    PROTECT(r_vec_names = allocVector(VECSXP, vec_len)); // allocate it as list since in Python it just has to be an unmuateable
    
    // TODO: check vec_len = 0

	// TODO: I return now NULL instead of list(NULL) which should also
        //       be possible I would some how set a NULL with class list
	if (r_vector_type == 10){                                           // boolean
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_VECTOR_ELT(r_vec_names, i, py_to_r(item, 0, 1));
            Py_XDECREF(item);
            item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
            Py_XDECREF(item);
            Py_XINCREF(item);
            // to handle NA variables of type None are transformed to NA
            if ( Py_GetR_Type(item) == 0 ){
				LOGICAL(r_vec)[i] = INT_MIN;
			}else{
				LOGICAL(r_vec)[i] = PY_TO_C_BOOLEAN(item);
			}
            // Py_XDECREF(item); Booleans never follow the api in Python!
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 13){                                   // integer
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);         
            SET_VECTOR_ELT(r_vec_names, i, py_to_r(item, 0, 1));
            Py_XDECREF(item);
            item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            // to handle NA variables of type None are transformed to NA
            if ( Py_GetR_Type(item) == 0 ){
				INTEGER(r_vec)[i] = INT_MIN;
			}else{
				INTEGER(r_vec)[i] = py_to_c_integer(item);
			}
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 14){                                   // numeric
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_VECTOR_ELT(r_vec_names, i, py_to_r(item, 0, 1));
            Py_XDECREF(item);
            item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            REAL(r_vec)[i] = PY_TO_C_DOUBLE(item);
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 16){                                 // character
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_VECTOR_ELT(r_vec_names, i, py_to_r(item, 0, 1));
            Py_XDECREF(item);
            item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_STRING_ELT(r_vec, i, mkCharCE(py_to_c_string(item), CE_UTF8));
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else{
        UNPROTECT(2);
        error("in py_dict_to_r_vec (ERROR CODE 0001)!\n");          // shouldn't happen!!
    }

    setAttrib(r_vec, R_NamesSymbol, r_vec_names);
    
    Py_XDECREF(py_keys);
    Py_XDECREF(py_values);

    UNPROTECT(2);
    return r_vec;
}

/*  ----------------------------------------------------------------------------

    py_dict_to_r_list

  ----------------------------------------------------------------------------*/
SEXP py_dict_to_r_list(PyObject *py_object, int simplify){
    PyObject *item, *py_keys, *py_values, *py_len, *py_i;
    SEXP r_list, r_list_names;
    long list_len;
    py_len = PyLong_FromSsize_t(PyDict_Size(py_object));
    list_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);

    item = NULL;

    py_keys = PyDict_Keys(py_object);
    py_values = PyDict_Values(py_object);

    PROTECT(r_list = allocVector(VECSXP, list_len));
    PROTECT(r_list_names = allocVector(VECSXP, list_len));

    for (long i=0; i < list_len; i++){
        // PyList_GetItem is a borrowed reference!
        py_i = PyLong_FromLong(i);

        item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        SET_VECTOR_ELT(r_list_names, i, py_to_r(item, 0, 1));
        Py_XDECREF(item);

        item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        SET_VECTOR_ELT(r_list, i, py_to_r(item, simplify, 1));
        Py_XDECREF(item);

        Py_XDECREF(py_i);
    }

    Py_XDECREF(py_keys);
    Py_XDECREF(py_values);

    setAttrib(r_list, R_NamesSymbol, r_list_names);

    UNPROTECT(2);
    return r_list;
}

/*  ----------------------------------------------------------------------------

    py_list_to_r_vec

  ----------------------------------------------------------------------------*/
SEXP py_list_to_r_vec(PyObject *py_object, int r_vector_type){
    PyObject *item, *py_len, *py_i;
    SEXP r_vec;
    long vec_len;
    
    if (r_vector_type == 0) return R_NilValue; // since you also can't create NULL vectors in R
    
    py_len = PyLong_FromSsize_t(PyList_GET_SIZE(py_object));
    vec_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);
    item = NULL;

    PROTECT(r_vec = allocVector(r_vector_type, vec_len));

    if (r_vector_type == 10){                                         // boolean
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            // to handle NA variables of type None are transformed to NA
            if ( Py_GetR_Type(item) == 0 ){
				LOGICAL(r_vec)[i] = INT_MIN;
			}else{
				LOGICAL(r_vec)[i] = PY_TO_C_BOOLEAN(item);
			}
            // Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 13){                                   // integer
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            // to handle NA variables of type None are transformed to NA
            if ( Py_GetR_Type(item) == 0 ){
				INTEGER(r_vec)[i] = INT_MIN;
			}else{
				INTEGER(r_vec)[i] = py_to_c_integer(item);
			}
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 14){                                   // numeric
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            REAL(r_vec)[i] = PY_TO_C_DOUBLE(item);
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 16){                                 // character
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_STRING_ELT(r_vec, i, mkCharCE(py_to_c_string(item), CE_UTF8));
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else{
        error("in py_list_to_r_vec\n");
    }

    UNPROTECT(1);

    return r_vec;
}


/*  ----------------------------------------------------------------------------
    
    py_tuple_to_r_vec

  ----------------------------------------------------------------------------*/
SEXP py_tuple_to_r_vec(PyObject *py_object, int r_vector_type){
    PyObject *item, *py_len, *py_i;
    SEXP r_vec;
    long vec_len;
    
    if (r_vector_type == 0) return R_NilValue; // since you also can't create NULL vectors in R
    
    py_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(py_object));
    vec_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);
    item = NULL;

    PROTECT(r_vec = allocVector(r_vector_type, vec_len));

    if (r_vector_type == 10){                                         // boolean
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            if ( Py_GetR_Type(item) == 0 ){
				LOGICAL(r_vec)[i] = INT_MIN;
			}else{
				LOGICAL(r_vec)[i] = PY_TO_C_BOOLEAN(item);
			}
            //Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 13){                                   // integer
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
			if ( Py_GetR_Type(item) == 0 ){
				INTEGER(r_vec)[i] = INT_MIN;
			}else{
				INTEGER(r_vec)[i] = py_to_c_integer(item);
			}
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 14){                                   // numeric
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            REAL(r_vec)[i] = PY_TO_C_DOUBLE(item);
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else if (r_vector_type == 16){                                 // character
        for (long i=0; i < vec_len; i++){
            py_i = PyLong_FromLong(i);
            item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
            Py_XINCREF(item);
            SET_STRING_ELT(r_vec, i, mkCharCE(py_to_c_string(item), CE_UTF8));
            Py_XDECREF(item);
            Py_DECREF(py_i);
        }
    }else{
        error("in py_list_to_r_vec\n");
    }

    UNPROTECT(1);

    return r_vec;
}

/*  ----------------------------------------------------------------------------

    py_list_to_r_list

  ----------------------------------------------------------------------------*/
SEXP py_list_to_r_list(PyObject *py_object, int simplify){
    PyObject *item, *py_len, *py_i;
    SEXP r_list;
    long list_len;
    py_len = PyLong_FromSsize_t(PyList_GET_SIZE(py_object));
    list_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);

    item = NULL;
    
    PROTECT(r_list = allocVector(VECSXP, list_len));

    for (long i=0; i < list_len; i++){
        // PyList_GetItem is a borrowed reference!
        py_i = PyLong_FromLong(i);

        item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        SET_VECTOR_ELT(r_list, i, py_to_r(item, simplify, 1));
        Py_XDECREF(item);

        Py_XDECREF(py_i);
    }

    UNPROTECT(1);
    return r_list;
}


/*  ----------------------------------------------------------------------------
    
    py_tuple_r_list

  ----------------------------------------------------------------------------*/
SEXP py_tuple_to_r_list(PyObject *py_object, int simplify){
    PyObject *item, *py_len, *py_i;
    SEXP r_list;
    long list_len;
    py_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(py_object));
    list_len = PY_TO_C_LONG(py_len);
    Py_XDECREF(py_len);

    item = NULL;
    
    PROTECT(r_list = allocVector(VECSXP, list_len));
    for (long i=0; i < list_len; i++){
        // PyTuple_GetItem is a borrowed reference!
        py_i = PyLong_FromLong(i);

        item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
        Py_XINCREF(item);
        SET_VECTOR_ELT(r_list, i, py_to_r(item, simplify, 1));
        Py_XDECREF(item);

        Py_DECREF(py_i);
    }
    
    UNPROTECT(1);
       
    return r_list;
}

long py_to_c_integer(PyObject *py_object){
    long c_long;
    if(PyInt_Check(py_object)){
        c_long = PY_TO_C_INTEGER(py_object);
    }else if(PyLong_Check(py_object)){
        c_long = PY_TO_C_LONG(py_object);
    }else{
        error("in py_to_r_integer!\n");
    }
    return c_long;
}

const char *py_to_c_string(PyObject *py_object){
    const char *c_string;
    if(PyString_Check(py_object)){
        c_string = PY_TO_C_STRING(py_object);
    }else if(PyUnicode_Check(py_object)){
        c_string = PY_TO_C_UNICODE(py_object);
    }else{
        error("in py_to_c_string!\n");
    }
    return c_string;
}

/*  ----------------------------------------------------------------------------

    py_to_r_simple 
      

  ----------------------------------------------------------------------------*/
SEXP py_to_r_simple(PyObject *py_object){
    SEXP r_val;
    if ( PyNone_Check(py_object) ){

        r_val =  R_NilValue;

    }else if( PyBool_Check(py_object) ){

        int c_boolean = PY_TO_C_BOOLEAN(py_object);
        r_val =  c_to_r_boolean(c_boolean);

    }else if(PyInt_Check(py_object)){

        long c_long = PY_TO_C_INTEGER(py_object);
        r_val =  c_to_r_integer(c_long);

    }else if(PyLong_Check(py_object)){

        long c_long = PY_TO_C_LONG(py_object);
        r_val =  c_to_r_integer(c_long);

    }else if(PyFloat_Check(py_object)){

        double c_double = PY_TO_C_DOUBLE(py_object);
        r_val =  c_to_r_double(c_double);

    }else if(PyString_Check(py_object)){
        const char *c_string;
        c_string = PY_TO_C_STRING(py_object);
        r_val =  c_to_r_string(c_string);

    }else if(PyUnicode_Check(py_object)){
        const char *c_string;
        c_string = PY_TO_C_UNICODE(py_object);
        r_val =  c_to_r_unicode(c_string);

    }else{

        Rprintf("error in py_to_r_simple can't convert this type!\n");
        r_val = py_class(py_object);

    }

    return r_val;
}

/*  ----------------------------------------------------------------------------

    py_to_r 
      
    in older version more python types are checked but not converted
  ----------------------------------------------------------------------------*/
SEXP py_to_r(PyObject *py_object, int simplify, int autotype){
    SEXP r_val;
    int r_type;

    if ( PyNone_Check(py_object) ){                                         // None
        r_val = R_NilValue;

    }else if( PyBool_Check(py_object) & autotype ){                         // Boolean
        int c_boolean;
        c_boolean = PY_TO_C_BOOLEAN(py_object);
        r_val =  c_to_r_boolean(c_boolean);

    }else if( PyInt_Check(py_object) & autotype ){                          // Integer
        long c_long;
        c_long = PY_TO_C_INTEGER(py_object);
        r_val = c_to_r_integer(c_long);

    }else if( PyLong_Check(py_object) & autotype ){                         // Long
        long c_long;
        c_long = PY_TO_C_LONG(py_object);
        r_val = c_to_r_integer(c_long);

    }else if( PyFloat_Check(py_object) & autotype ){                        // Float
        double c_double;
        c_double = PY_TO_C_DOUBLE(py_object);
        r_val = c_to_r_double(c_double);

    }else if( PyString_Check(py_object) & autotype ){                       // String
        const char *c_string;
        c_string = PY_TO_C_STRING(py_object);
        r_val = c_to_r_string(c_string);

    }else if( PyUnicode_Check(py_object) & autotype ){                      // Unicode
        const char *c_string;
        c_string = PY_TO_C_UNICODE(py_object);
        r_val = c_to_r_unicode(c_string); 

    }else if( PyTuple_Check(py_object) & autotype ){                        // Tuple
        if (simplify){
	       r_type = PyTuple_AllSameType(py_object);
	       if ( r_type > -1 ){  
	           r_val = py_tuple_to_r_vec(py_object, r_type);
	       }else{
   	           r_val = py_tuple_to_r_list(py_object, simplify);
	       }
        }else{
	    r_val = py_tuple_to_r_list(py_object, simplify);
	    }
    }else if( PyList_Check(py_object) & autotype ){                         // List
        if (simplify){
            r_type = PyList_AllSameType(py_object);
	        if ( r_type > -1 ){
 	            r_val = py_list_to_r_vec(py_object, r_type);
	        }else{
	            r_val = py_list_to_r_list(py_object, simplify);
            }
        }else{
	        r_val = py_list_to_r_list(py_object, simplify);
	    }
    }else if( PyDict_Check(py_object) & autotype ){                         // Dict		
        if (simplify){
	        r_type = PyDict_AllSameType(py_object);
	        if ( r_type > -1 ){
	            r_val = py_dict_to_r_vec(py_object, r_type);
	        }else{
	          r_val = py_dict_to_r_list(py_object, simplify);
	        }
        }else{
	        r_val = py_dict_to_r_list(py_object, simplify);
        }
    }else{
        //const char * c_type;
        
        r_val = py_class(py_object);
        
		//PyObject *py_type;
		//py_type = PyObject_Type(py_object);

		
		/* TODO:
		 * 	1) If I can't convert the object I don't convert it 
		 *     but assign it to a dictonary of the class PythonInR
		 *     called rNamespace. Therefore only the key in the
		 *     dictionary gets returned like a pointer.
		 *  2) 
		 * 
		 */
		//c_type = R_TO_C_STRING(py_obj_str(py_type));
		//Py_XDECREF(py_type);
		//<<<TODO: add flag to turn of this warning!
        //   could be 3 modes like in the Python encoding confersion 
        //   -) strict -> error 
        //   -) warning -> warning
        //   -) ignore -> return type as string >>>
        // warning("can't convert objects of type %s!", c_type);
    }
    
    return r_val;
}

