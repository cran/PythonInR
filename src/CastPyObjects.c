/*  ----------------------------------------------------------------------------
 
 PyToRbasic

Provides functions to convert non nested Python data types to R data types.

----------------------------------------------------------------------------*/

#include "CastPyObjects.h"
#include "PythonInR.h"
#include "CToR.h"

const int LIST_COLLECTION_TYPE = 0;
const int TUPLE_COLLECTION_TYPE = 1;
const int R_UNCATEGORIZED_TYPE = -1;

/*
* Helper Functions
*/

/*  ----------------------------------------------------------------------------

* convert
*   converts one item in a python collection to an item in an r vector
*
*  params
*    r_vector_type 
*      the type of the result r vector
*    r_vec 
*      the result r vector
*    pos
*      the position of the item to convert into r_vec
*    item
*      the item in to convert

----------------------------------------------------------------------------*/
void convert(int r_vector_type, SEXP r_vec, long pos, PyObject *item) {
  if ( Py_GetR_Type(item) == NILSXP ){
    if (r_vector_type == LGLSXP) {
      LOGICAL(r_vec)[pos] = NA_LOGICAL;
    } else if (r_vector_type == INTSXP) {
      INTEGER(r_vec)[pos] = NA_INTEGER;
    } else if (r_vector_type == REALSXP) {
      REAL(r_vec)[pos] = NA_REAL;
    } else if (r_vector_type == STRSXP) {
      SET_STRING_ELT(r_vec, pos, NA_STRING);
    } else {
      error("do not support r vector type ", r_vector_type);
    }
  } else if (r_vector_type == LGLSXP) {
    LOGICAL(r_vec)[pos] = PY_TO_C_BOOLEAN(item);
  } else if (r_vector_type == INTSXP) {
    INTEGER(r_vec)[pos] = PY_TO_C_INTEGER(item);
  } else if (r_vector_type == REALSXP) {
    REAL(r_vec)[pos] = PY_TO_C_DOUBLE(item);
  } else if (r_vector_type == STRSXP) {
    SET_STRING_ELT(r_vec, pos, mkCharCE(py_to_c_string(item), CE_UTF8));
  }
}

/*  ----------------------------------------------------------------------------

* convert_vector
*   converts a python collection to an r vector
*
*   params
*    vec_len
*      the length of the vector
*    py_keys
*      the python collection that will be converted into a name vector,
*      could be NULL
*    py_values
*      the python collection that will be converted into an r vector
*    r_vec
*      the returning r vector
*    r_vec_names
*      the returning name vector,
*      could be NULL
*    r_type
*      the type of r_vec
*    collection_type
*      the type of the python collection that we are converting

----------------------------------------------------------------------------*/
void convert_vector(long vec_len, PyObject *py_keys, PyObject *py_values,
                    SEXP r_vec, SEXP r_vec_names, int r_type, int collection_type) {
  PyObject *key_item, *value_item, *py_i;
  for (long i = 0; i < vec_len; i++){
    py_i = PyLong_FromLong(i);
    
    if (r_vec_names != NULL) {
      key_item = PyList_GetItem(py_keys, PyLong_AsSsize_t(py_i));
      Py_XINCREF(key_item);
      SET_VECTOR_ELT(r_vec_names, i, py_to_r(key_item, 0, 1));
      Py_XDECREF(key_item);
    }
    
    if (collection_type == LIST_COLLECTION_TYPE) {
      value_item = PyList_GetItem(py_values, PyLong_AsSsize_t(py_i));
    } else if (collection_type == TUPLE_COLLECTION_TYPE) {
      value_item = PyTuple_GetItem(py_values, PyLong_AsSsize_t(py_i));
    } else {
      error("do not support collection type ", collection_type);
    }
    Py_XINCREF(value_item);
    convert(r_type, r_vec, i, value_item);
    Py_XDECREF(value_item);
    Py_DECREF(py_i);
  }
}


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

Increases the reference count from py_object + 1

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
  int r_type = R_UNCATEGORIZED_TYPE;
  
  if ( PyNone_Check(py_object) ){
    r_type = NILSXP;
  }else if ( PyBool_Check(py_object) ){
    r_type = LGLSXP;
  }else if ( PyInt_Check(py_object) ){
    r_type = REALSXP;
  }else if ( PyLong_Check(py_object) ){
    r_type = REALSXP;
  }else if ( PyFloat_Check(py_object) ){
    r_type = REALSXP;
  }else if ( PyString_Check(py_object) ){
    r_type = STRSXP;
  }else if ( PyUnicode_Check(py_object) ){
    r_type = STRSXP;
  }
  return r_type;
}

/*  ----------------------------------------------------------------------------
PyCollection_AllSameType
params
py_object
the python collection to check
collection_type
the type of py_object,
use LIST_COLLECTION_TYPE or TUPLE_COLLECTION_TYPE
returns
the integer that Py_GetR_Type() returns for all items in the list (except None),
or R_UNCATEGORIZED_TYPE if the type can't be determined because either items in 
the list do not have the same type or if the list is empty.
--------------------------------------------------------------------------*/
int PyCollection_AllSameType(PyObject *py_object, int collection_type){
  PyObject *item, *py_len, *py_i;
  
  if (Py_GetR_Type(py_object) == NILSXP) return NILSXP;
  
  if (collection_type == LIST_COLLECTION_TYPE) {
    py_len = PyLong_FromSsize_t(PyList_GET_SIZE(py_object));
  } else if (collection_type == TUPLE_COLLECTION_TYPE) {
    py_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(py_object));
  } else {
    // should throw an error here
    return R_UNCATEGORIZED_TYPE;
  }
  
  long list_len = PY_TO_C_LONG(py_len);
  Py_XDECREF(py_len);
  
  // empty list will be converted to empty list in r
  if (list_len == 0) {
    return R_UNCATEGORIZED_TYPE;
  }
  
  int r_type = NILSXP;
  long i = 0;
  while (i < list_len) {
    py_i = PyLong_FromLong(i);
    if (collection_type == LIST_COLLECTION_TYPE) {
      item = PyList_GetItem(py_object, PyLong_AsSsize_t(py_i));
    } else if (collection_type == TUPLE_COLLECTION_TYPE) {
      item = PyTuple_GetItem(py_object, PyLong_AsSsize_t(py_i));
    } else {
      error("do not support collection type ", collection_type);
    }
    Py_XINCREF(item);
    Py_XDECREF(py_i);
    int item_type = Py_GetR_Type(item);
    if (item_type == R_UNCATEGORIZED_TYPE) {
      return R_UNCATEGORIZED_TYPE;
    }
    if ((r_type > NILSXP) && (item_type > NILSXP) && (item_type != r_type)) {
      return R_UNCATEGORIZED_TYPE;
    }
    if ((r_type == NILSXP) && (item_type > NILSXP)) {
      r_type = item_type;
    }
    Py_XDECREF(item);
    i++;
  }
  
  if (r_type == NILSXP) {
    // there is None item in the list
    // return a list with NA (logical)
    return LGLSXP;
  } else {
    return r_type;
  }
}

/*  ----------------------------------------------------------------------------
PyDict_AllSameType
--------------------------------------------------------------------------*/
int PyDict_AllSameType(PyObject *py_object){
  PyObject *py_values;
  int r_type;
  // PyDict_Values -> New reference
  py_values = PyDict_Values(py_object);
  r_type = PyCollection_AllSameType(py_values, LIST_COLLECTION_TYPE);
  Py_XDECREF(py_values);
  return  r_type;
}

/*  ----------------------------------------------------------------------------

py_dict_to_r_vec

----------------------------------------------------------------------------*/
SEXP py_dict_to_r_vec(PyObject *py_object, int r_vector_type){
  PyObject *py_keys, *py_values, *py_len;
  SEXP r_vec, r_vec_names;
  long vec_len;
  
  if (r_vector_type == NILSXP) return R_NilValue;
  
  py_len = PyLong_FromSsize_t(PyDict_Size(py_object));
  vec_len = PY_TO_C_LONG(py_len);
  Py_XDECREF(py_len);
  py_keys = PyDict_Keys(py_object);
  py_values = PyDict_Values(py_object);
  
  PROTECT(r_vec = allocVector(r_vector_type, vec_len));
  PROTECT(r_vec_names = allocVector(VECSXP, vec_len)); // allocate it as list since in Python it just has to be an unmuateable
  
  if (r_vector_type > NILSXP) {
    convert_vector(vec_len, py_keys, py_values, r_vec, r_vec_names, r_vector_type, LIST_COLLECTION_TYPE);
  } else{
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
  PyObject *py_len;
  SEXP r_vec;
  long vec_len;
  
  if (r_vector_type == NILSXP) return R_NilValue; // since you also can't create NULL vectors in R
  
  py_len = PyLong_FromSsize_t(PyList_GET_SIZE(py_object));
  vec_len = PY_TO_C_LONG(py_len);
  Py_XDECREF(py_len);
  
  PROTECT(r_vec = allocVector(r_vector_type, vec_len));
  
  if (r_vector_type > NILSXP) {
    convert_vector(vec_len, NULL, py_object, r_vec, NULL, r_vector_type, LIST_COLLECTION_TYPE);
  } else{
    error("in py_list_to_r_vec\n");
  }
  
  UNPROTECT(1);
  
  return r_vec;
}


/*  ----------------------------------------------------------------------------

py_tuple_to_r_vec

----------------------------------------------------------------------------*/
SEXP py_tuple_to_r_vec(PyObject *py_object, int r_vector_type){
  PyObject *py_len;
  SEXP r_vec;
  long vec_len;
  
  if (r_vector_type == NILSXP) return R_NilValue; // since you also can't create NULL vectors in R
  
  py_len = PyLong_FromSsize_t(PyTuple_GET_SIZE(py_object));
  vec_len = PY_TO_C_LONG(py_len);
  Py_XDECREF(py_len);
  
  PROTECT(r_vec = allocVector(r_vector_type, vec_len));
  
  if (r_vector_type > NILSXP) {
    convert_vector(vec_len, NULL, py_object, r_vec, NULL, r_vector_type, TUPLE_COLLECTION_TYPE);
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

int py_to_c_integer(PyObject *py_object){
  long c_long;
  if(PyInt_Check(py_object)){
    c_long = PY_TO_C_INTEGER(py_object);
  }else if(PyLong_Check(py_object)){
    c_long = PY_TO_C_LONG(py_object);
  }else{
    error("in py_to_r_integer!\n");
  }
  return (int)c_long;
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
    double c_double;
    c_double = PY_TO_C_DOUBLE(py_object);
    r_val = c_to_r_double(c_double);
    
  }else if( PyLong_Check(py_object) & autotype ){                         // Long
    double c_double;
    c_double = PY_TO_C_DOUBLE(py_object);
    r_val = c_to_r_double(c_double);
    
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
    
  }else if( PyTuple_CheckExact(py_object) & autotype ){                        // Tuple
    if (simplify){
      r_type = PyCollection_AllSameType(py_object, TUPLE_COLLECTION_TYPE);
      if ( r_type > -1 ){  
        r_val = py_tuple_to_r_vec(py_object, r_type);
      }else{
        r_val = py_tuple_to_r_list(py_object, simplify);
      }
    }else{
      r_val = py_tuple_to_r_list(py_object, simplify);
    }
  }else if( PyList_CheckExact(py_object) & autotype ){                         // List
    if (simplify){
      r_type = PyCollection_AllSameType(py_object, LIST_COLLECTION_TYPE);
      if ( r_type > -1 ){
        r_val = py_list_to_r_vec(py_object, r_type);
      }else{
        r_val = py_list_to_r_list(py_object, simplify);
      }
    }else{
      r_val = py_list_to_r_list(py_object, simplify);
    }
  }else if( PyDict_CheckExact(py_object) & autotype ){                         // Dict
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
