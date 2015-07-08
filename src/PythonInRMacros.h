/* --------------------------------------------------------------------------  \
    
   PythonInRUnix
   
   Define some macros for type casting.
   This header is used in all other files!
   
\  -------------------------------------------------------------------------- */

#ifndef PYTHON_IN_R_UNIX
#define PYTHON_IN_R_UNIX
   
#define xstr(a) str(a)
#define str(a) #a

#define PY_NONE Py_BuildValue("")

////////////////////////////////////////////////////////////////////////
// Define some makros to convert R primitives to C
////////////////////////////////////////////////////////////////////////
#define R_TO_C_BOOLEAN(b) LOGICAL(b)[0]

#ifdef _WIN32
    #define R_TO_C_STRING(s)  reEnc(CHAR(STRING_ELT(s, 0)), getCharCE(STRING_ELT(s, 0)), CE_UTF8, 1)
    #define R_TO_C_STRING_V(s,i) reEnc(CHAR(STRING_ELT(s, (long)i)), getCharCE(STRING_ELT(s, (long)i)), CE_UTF8, 1)
#else
    #define R_TO_C_STRING(s)  CHAR(STRING_ELT(s, 0))
    #define R_TO_C_STRING_V(s,i)    CHAR(STRING_ELT(s, (long)i))
#endif

#define R_TO_C_INT(n)     INTEGER(n)[0]
#define R_TO_C_LONG(n)    (long)INTEGER(n)[0]
#define R_TO_C_DOUBLE(n)  REAL(n)[0]

#define R_TO_C_BOOLEAN_V(b,i)   LOGICAL(b)[i]
#define R_TO_C_LONG_V(n,i)      (long)INTEGER(n)[i]
#define R_TO_C_DOUBLE_V(n,i)    REAL(n)[i]

////////////////////////////////////////////////////////////////////////
// Define some makros to convert R primitives to Python
////////////////////////////////////////////////////////////////////////
// NOTE: integer hat kein Inf und NaN as.integer(Inf) und as.integer(NaN)
//       ergibt beides NA
#define R_TO_PY_BOOLEAN(b) ( R_TO_C_BOOLEAN(b) == NA_LOGICAL ) ? PY_NONE : PyBool_FromLong((long)R_TO_C_BOOLEAN(b))
#define R_TO_PY_LONG(n)    ( R_TO_C_INT(n)     == NA_INTEGER ) ? PY_NONE : PyLong_FromLong(R_TO_C_LONG(n))
#define R_TO_PY_DOUBLE(n)  PyFloat_FromDouble(R_TO_C_DOUBLE(n))


#if (PY_MAJOR_VERSION >= 3 || (!defined(PYTHON_IN_R_NO_EXPLICIT_LINKING)))
    #define R_TO_PY_STRING(s)  PyBytes_FromString(R_TO_C_STRING(s))
#else
    #define R_TO_PY_STRING(s)  PyString_FromString(R_TO_C_STRING(s))
#endif
#define R_TO_PY_UNICODE(s) PyUnicode_FromString(R_TO_C_STRING(s))

#define R_TO_PY_BOOLEAN_V(b,i) ( R_TO_C_BOOLEAN_V(b, i) == NA_LOGICAL ) ? PY_NONE : PyBool_FromLong((long)R_TO_C_BOOLEAN_V(b,i))
#define R_TO_PY_LONG_V(n,i)    ( R_TO_C_LONG_V(n,i)      == NA_INTEGER ) ? PY_NONE : PyLong_FromLong(R_TO_C_LONG_V(n,i))
#define R_TO_PY_DOUBLE_V(n,i)  PyFloat_FromDouble(R_TO_C_DOUBLE_V(n,i))

#define R_TO_PY_STRING_V(s,i)  PyBytes_FromString(R_TO_C_STRING_V(s,i))
#define C_TO_PY_STRING_V(s,i)  PyBytes_FromString(s,i)
#define R_TO_PY_UNICODE_V(s,i) PyUnicode_FromString(R_TO_C_STRING_V(s,i))

////////////////////////////////////////////////////////////////////////
// Define some makros to convert Python primitives to C
////////////////////////////////////////////////////////////////////////
#define PY_TO_C_BOOLEAN(b)  PyObject_IsTrue(b)
#define PY_TO_C_STRING(s)   PyString_AsString(s)
#define PY_TO_C_UNICODE(s)  PyBytes_AsString(PyUnicode_AsUTF8String(s))

#if (PY_MAJOR_VERSION >= 3  || (!defined(PYTHON_IN_R_NO_EXPLICIT_LINKING)))
    #define PY_TO_C_INTEGER(n)  PyLong_AsLong(n)
#else
    #define PY_TO_C_INTEGER(n)  PyInt_AS_LONG(n)
#endif

#define PY_TO_C_LONG(n)     PyLong_AsLong(n)
#define PY_TO_C_DOUBLE(n)   PyFloat_AsDouble(n) 

////////////////////////////////////////////////////////////////////////
// Define some makros to convert Python primitives to R
////////////////////////////////////////////////////////////////////////
#define PY_TO_R_BOOLEAN(b) (int)PY_TO_C_BOOLEAN(b)
#define PY_TO_R_STRING(s)  mkChar(PY_TO_C_STRING(s))
#define PY_TO_R_UNICODE(s) mkCharCE(PY_TO_C_UNICODE(s), CE_UTF8)

#define REF_CNT(ob) (PyLong_AsLong(PyLong_FromSsize_t(Py_REFCNT(ob))))
#define DPRC(obj) Rprintf("#obj: %i", REF_CNT(obj))


#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING
    #define PyNone_Check(o) (o == Py_None)
    #if PY_MAJOR_VERSION >= 3
        // ---------------------------------------------------------------------
        // Python 3 
        // ---------------------------------------------------------------------
        #define PY_V_CHAR(s) (wchar_t *)s
        #define PY_V_CHAR2(s) PyUnicode_FromString(s)
        #define PY_TO_C_OBJECT_STRING PY_TO_C_UNICODE
        //#define PyString_FromString(s) PyUnicode_FromFormat("%s", s)
        #define PyString_AsString PyBytes_AsString
        #define PyString_Check(i) PyBytes_Check(i)
        #define PyUnicode_FromConstChar(s) PyUnicode_FromFormat("%s", s)
        #define PyInt_Check(i) PyLong_Check(i)
        #define PyInt_FromLong(s) PyLong_FromLong(s)
        #define PyInt_AsLong(s) PyLong_AsLong(s)
        #define PyInt_FromSsize_t(s) PyLong_FromSsize_t(s)
        #define PyClass_Check(obj) PyObject_IsInstance(obj, (PyObject *)&PyType_Type)
        #define PyInternalString_FromString(s) PyUnicode_FromString(s)
    #else
        // ---------------------------------------------------------------------
        // Python tested with 2.7 but should work also with earlier versions
        // ---------------------------------------------------------------------
        #define PY_V_CHAR(s) s
        #define PY_V_CHAR2(s) s
        #define PY_TO_C_OBJECT_STRING PY_TO_C_STRING
        #define PyUnicode_FromConstChar(s) PyUnicode_FromString(s)
        #define PyInternalString_FromString(s) PyString_FromString(s)
    #endif    
#endif

#endif
