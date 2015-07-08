/* --------------------------------------------------------------------------  \

    CastRObjects

    Provides functions cast R objects into Python objects!

\  -------------------------------------------------------------------------- */

#include "CastRObjects.h"
#include "PythonInR.h"

/*  ----------------------------------------------------------------------------

    r_logical_to_py_boolean 

  ----------------------------------------------------------------------------*/
PyObject *r_logical_to_py_boolean(SEXP r_object){
    PyObject *py_object;
    if( IS_LOGICAL(r_object) ){
        py_object = R_TO_PY_BOOLEAN(r_object);
    } else{
        error("r_logical_to_py_boolean the provided variable has not type logical!");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_integer_to_py_long 

  ----------------------------------------------------------------------------*/
PyObject *r_integer_to_py_long(SEXP r_object){
    PyObject *py_object;
    if( IS_INTEGER(r_object) ){
        py_object = R_TO_PY_LONG(r_object);
    } else{
        error("r_integer_to_py_long the provided variable has not type integer!");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_numeric_to_py_double 

  ----------------------------------------------------------------------------*/
PyObject *r_numeric_to_py_double(SEXP r_object){
    PyObject *py_object;
    if( IS_NUMERIC(r_object) ){
        py_object = R_TO_PY_DOUBLE(r_object);
    } else{
        error("r_numeric_to_py_double the provided variable has not type numeric!");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_character_to_py_string
      is byte string in python 3
  ----------------------------------------------------------------------------*/
PyObject *r_character_to_py_string(SEXP r_object){
    PyObject *py_object;
    if( IS_CHARACTER(r_object) ){
        py_object = R_TO_PY_STRING(r_object);
    } else{
        error("r_character_to_py_string the provided variable has not type character!");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_character_to_py_unicode

  ----------------------------------------------------------------------------*/
PyObject *r_character_to_py_unicode(SEXP r_object){
    PyObject *py_object;
    if( IS_CHARACTER(r_object) ){
        py_object = R_TO_PY_UNICODE(r_object);
    } else{
        error("r_character_to_py_unicode the provided variable has not type character!");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_to_py_primitive

  ----------------------------------------------------------------------------*/
PyObject *r_to_py_primitive(SEXP r_object){
    PyObject *py_object;
    
    if( IS_LOGICAL(r_object) ){
        py_object = R_TO_PY_BOOLEAN(r_object);
    } else if ( IS_INTEGER(r_object) ){
        py_object = R_TO_PY_LONG(r_object);
    } else if ( IS_NUMERIC(r_object) ){
        py_object = R_TO_PY_DOUBLE(r_object);
    } else if ( IS_CHARACTER(r_object) ){
        py_object = R_TO_PY_UNICODE(r_object);
    } else if ( isComplex(r_object) ){
        error("in r_to_py_primitive\n     conversion of type complex isn't supported jet!");
    } else {
        error("in r_to_py_primitive\n     unkown data type!\n\n");
    }
    return py_object;   
}

/*  ----------------------------------------------------------------------------

    r_to_py_tuple

  ----------------------------------------------------------------------------*/
PyObject *r_to_py_tuple(SEXP r_object){
    PyObject *py_object, *item;
    long i, len;

    len = GET_LENGTH(r_object);
    py_object = PyTuple_New(len);

    if (len == 0) return(py_object);
    
    if( IS_LOGICAL(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_BOOLEAN_V(r_object,i);
            PyTuple_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_INTEGER(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_LONG_V(r_object,i);
            PyTuple_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_NUMERIC(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_DOUBLE_V(r_object,i);
            PyTuple_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_CHARACTER(r_object) ){
       for(i = 0; i < len; i++) {
            item = R_TO_PY_UNICODE_V(r_object,i);
            PyTuple_SET_ITEM(py_object, i, item);
       }
    }else if ( isComplex(r_object) ){
        Py_XDECREF(py_object);
        error("in r_to_py_tuple\n     conversion of type complex isn't supported jet!");
    }else if ( IS_LIST(r_object) ){
        for(i = 0; i < len; i++) {
            item = r_to_py(VECTOR_ELT(r_object, i));
            PyTuple_SET_ITEM(py_object, i, item);
        }
    }else {
        Py_XDECREF(py_object);
        error("in r_to_py_tuple\n     unkown data type!\n\n");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_to_py_list

  ----------------------------------------------------------------------------*/
PyObject *r_to_py_list(SEXP r_object){
    PyObject *py_object, *item;
    long i, len;

    len = GET_LENGTH(r_object);
    py_object = PyList_New(len);
    
    if( IS_LOGICAL(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_BOOLEAN_V(r_object,i);
            PyList_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_INTEGER(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_LONG_V(r_object,i);
            PyList_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_NUMERIC(r_object) ){
        for(i = 0; i < len; i++) {
            item = R_TO_PY_DOUBLE_V(r_object,i);
            PyList_SET_ITEM(py_object, i, item);
        }
    }else if ( IS_CHARACTER(r_object) ){
       for(i = 0; i < len; i++) {
            item = R_TO_PY_UNICODE_V(r_object,i);
            PyList_SET_ITEM(py_object, i, item);
       }
    }else if ( isComplex(r_object) ){
        Py_XDECREF(py_object);
        error("in r_to_py_list\n     conversion of type complex isn't supported jet!");
    }else if ( IS_LIST(r_object) ){
        for(i = 0; i < len; i++) {
            item = r_to_py(VECTOR_ELT(r_object, i));
            PyList_SET_ITEM(py_object, i, item);
        }
    }else {
        Py_XDECREF(py_object);
        error("in r_to_py_list\n     unkown data type!\n\n");
    }
    return py_object;
}

/*  ----------------------------------------------------------------------------

    r_to_py_dict

  ----------------------------------------------------------------------------*/
PyObject *r_to_py_dict(SEXP r_keys, SEXP r_values){
    PyObject *py_object, *key, *value;
    long i, len;

    len = GET_LENGTH(r_values);
    py_object = PyDict_New();

    if( IS_LOGICAL(r_values) ){
        for(i = 0; i < len; i++) {
            key = R_TO_PY_UNICODE_V(r_keys,i);
            value = R_TO_PY_BOOLEAN_V(r_values,i);
            PyDict_SetItem(py_object, key, value);
        }
    }else if ( IS_INTEGER(r_values) ){
        for(i = 0; i < len; i++) {
            key = R_TO_PY_UNICODE_V(r_keys,i);
            value = R_TO_PY_LONG_V(r_values,i);
            PyDict_SetItem(py_object, key, value);
        }
    }else if ( IS_NUMERIC(r_values) ){
        for(i = 0; i < len; i++) {
            key = R_TO_PY_UNICODE_V(r_keys,i);
            value = R_TO_PY_DOUBLE_V(r_values,i);
            PyDict_SetItem(py_object, key, value);
        }
    }else if ( IS_CHARACTER(r_values) ){
        for(i = 0; i < len; i++) {
            key = R_TO_PY_UNICODE_V(r_keys,i);
            value = R_TO_PY_UNICODE_V(r_values,i);
            PyDict_SetItem(py_object, key, value);
        }
    }else if ( isComplex(r_values) ){
        Py_XDECREF(py_object);
        error("in r_to_py_dict\n     conversion of type complex isn't supported jet!");
    }else if ( IS_LIST(r_values) ){
        for(i = 0; i < len; i++) {
            key = R_TO_PY_UNICODE_V(r_keys,i);
            value = r_to_py(VECTOR_ELT(r_values, i));
            PyDict_SetItem(py_object, key, value);
        }
    }else {
        Py_XDECREF(py_object);
        error("in r_to_py_dict\n     unkown data type!\n\n");
    }    
    return py_object;
}

/* A function to explore R objects where it is not clear to me what
   they made of at a C level.                                       */
SEXP test_r_flags(SEXP x){
    Rprintf("IS_NULL: %i\n", x==NULL);
    Rprintf("IS_LOGICAL: %i\n", IS_LOGICAL(x));
    Rprintf("IS_INTEGER: %i\n", IS_INTEGER(x));
    Rprintf("IS_NUMERIC: %i\n", IS_NUMERIC(x));
    Rprintf("IS_CHARACTER: %i\n", IS_CHARACTER(x));
    Rprintf("isComplex: %i\n", isComplex(x));
    Rprintf("IS_LIST: %i\n", IS_LIST(x));

    Rprintf("isArray       : %i\n", isArray(x));
    Rprintf("isComplex     : %i\n", isComplex(x));
    Rprintf("isEnvironment : %i\n", isEnvironment(x));
    Rprintf("isExpression  : %i\n", isExpression(x));
    Rprintf("isFactor      : %i\n", isFactor(x));
    Rprintf("isFrame       : %i\n", isFrame(x));
    Rprintf("isFunction    : %i\n", isFunction(x));
    Rprintf("isInteger     : %i\n", isInteger(x));
    Rprintf("isLanguage    : %i\n", isLanguage(x));
    Rprintf("isList        : %i\n", isList(x));
    Rprintf("isLogical     : %i\n", isLogical(x));
    Rprintf("isSymbol      : %i\n", isSymbol(x));
    Rprintf("isMatrix      : %i\n", isMatrix(x));
    Rprintf("isNewList     : %i\n", isNewList(x));
    Rprintf("isNull        : %i\n", isNull(x));
    Rprintf("isNumeric     : %i\n", isNumeric(x));
    Rprintf("isNumber      : %i\n", isNumber(x));
    Rprintf("isObject      : %i\n", isObject(x));
    Rprintf("isOrdered     : %i\n", isOrdered(x));
    Rprintf("isPairList    : %i\n", isPairList(x));
    Rprintf("isPrimitive   : %i\n", isPrimitive(x));
    Rprintf("isReal        : %i\n", isReal(x));
    Rprintf("isS4          : %i\n", isS4(x));
    Rprintf("isString      : %i\n", isString(x));
    Rprintf("isTs          : %i\n", isTs(x));
    Rprintf("isUnordered   : %i\n", isUnordered(x));
    Rprintf("isUserBinop   : %i\n", isUserBinop(x));
    Rprintf("isValidString : %i\n", isValidString(x));
    Rprintf("isValidStringF: %i\n", isValidStringF(x));
    Rprintf("isVector      : %i\n", isVector(x));
    Rprintf("isVectorAtomic: %i\n", isVectorAtomic(x));
    Rprintf("isVectorizable: %i\n", isVectorizable(x));
    Rprintf("isVectorList  : %i\n", isVectorList(x));

    return R_NilValue;
}

/*  ----------------------------------------------------------------------------

    isPythonInRObject
    
    Returns True if x is a PythonInR PyObject.
    
  ----------------------------------------------------------------------------*/
int isPyInR_PyObject(SEXP x){
    int is_py_in_r_obj = 0;
    if ( isS4(x) ) return(is_py_in_r_obj);
    SEXP cls = getAttrib(x, R_ClassSymbol);
    if (IS_CHARACTER(cls)){
		int i = GET_LENGTH(cls) - 2; // -2 da letztes element R6 und von 0 weg
		if (i >= 0){
			is_py_in_r_obj = (strncmp(R_TO_C_STRING_V(cls, i), PythonInR_PyObjectString, 8) == 0);
		}
    }
    return is_py_in_r_obj;
}

int compare_r_class(SEXP x, const char *className){
    int retVal = 0;
    SEXP cls = getAttrib(x, R_ClassSymbol);
    if (IS_CHARACTER(cls)){
		retVal = (strcmp(R_TO_C_STRING_V(cls, 0), className) == 0);
    }
    return retVal;
}

const char *r_get_py_object_location(SEXP x){
	SEXP cx, names;
	int i, len;
	
	x = CAR(x);
	names = GET_NAMES(x);
	len = GET_LENGTH(x);
	
	for(i = 0; i < len; i++) {
		if ( strcmp(R_TO_C_STRING_V(names, i), "py.variableName") == 0 ){
			cx = nthcdr(x, (int) i);
			x = CAR(cx);
			return R_TO_C_STRING(x);
		}
	}
	return NULL;
}

/*  ----------------------------------------------------------------------------

    r_to_py
    
    Returns New reference! 
    
  ----------------------------------------------------------------------------*/
PyObject *r_to_py(SEXP r_object){
    PyObject *py_object;
    long len=-1; 
    SEXP names;
      
	if ( isPyInR_PyObject(r_object) ){
		const char *py_obj_name = r_get_py_object_location(r_object);
		if ( py_obj_name == NULL) error("PythonInR object is not valid!");
		py_object = py_get_py_obj( py_obj_name );
		return py_object;
	} else if ( compare_r_class(r_object, "tuple") ) {
		py_object = r_to_py_tuple(r_object);
		return py_object;
	}
    
    len = GET_LENGTH(r_object);
    names = GET_NAMES(r_object);
    
    if(GET_LENGTH(names) > 0){                                                  // Case 1: R object has names!        
        py_object = r_to_py_dict(names, r_object);
    }else if ( len == 1 && ( IS_LOGICAL(r_object) || IS_INTEGER(r_object) || 
                             IS_NUMERIC(r_object) || IS_CHARACTER(r_object) || 
                             isComplex(r_object)) ){                            // Case 2: Convert to int, unicode, ...!
        py_object = r_to_py_primitive(r_object);
    }else if ( (IS_LIST(r_object) && len >= 1) ) {                              // Case 3: R object is a list!
        py_object = r_to_py_list(r_object);
    }else if ( len == 0 ){                                                      // Case 4: Convert R NULL or character(0), ... to Py_None
        Py_RETURN_NONE; // Return value: New reference.
    }else{
        error("the provided R object can not be type cast into an Python object\n"); 
    }
    return py_object;
}
