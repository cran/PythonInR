#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING
int ISO_C_forbids_an_empty_translation_unit;
#else
#ifndef PYTHON_EXPLICIT_LINKING
#define PYTHON_EXPLICIT_LINKING

int PYTHON_API_VERSION; /* Start with the number of Python 2.79 (could be anyone) */
int PYTHON_MAJOR_VERSION; /* Could get a bug one day ;) */

#define PY_MAJOR_VERSION 9999

#define Py_single_input 256
#define Py_file_input 257
#define Py_eval_input 258

#define METH_VARARGS  0x0001

#define _PyObject_HEAD_EXTRA
#define _PyObject_EXTRA_INIT

//typedef long            Py_intptr_t;
typedef long            Py_intptr_t;
typedef Py_intptr_t     Py_ssize_t;
//typedef int Py_ssize_t;

/* PyObject_HEAD defines the initial segment of every PyObject. */
#define PyObject_HEAD                   \
    _PyObject_HEAD_EXTRA                \
    Py_ssize_t ob_refcnt;               \
    struct _typeobject *ob_type;
    
#define PyObject_VAR_HEAD               \
    PyObject_HEAD                       \
    Py_ssize_t ob_size; /* Number of items in variable part */

typedef struct _typeobject {
    PyObject_VAR_HEAD
    const char *tp_name; /* For printing, in format "<module>.<name>" */
    Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */

} PyTypeObject;
    
typedef struct _object {
    PyObject_HEAD
} PyObject;

typedef struct {
    PyObject_VAR_HEAD
} PyVarObject;

typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);

struct PyMethodDef { // Is the same for Python 3.4 and 2.7
    const char	*ml_name;	/* The name of the built-in function/method */
    PyCFunction  ml_meth;	/* The C function that implements it */
    int		 ml_flags;	/* Combination of METH_xxx flags, which mostly
				   describe the args expected by the C func */
    const char	*ml_doc;	/* The __doc__ attribute, or NULL */
};
typedef struct PyMethodDef PyMethodDef;

// For Python 3 we need different initialization macros
#define PyObject_HEAD3                   PyObject ob_base;

#define PyObject_HEAD_INIT(type)        \
    { _PyObject_EXTRA_INIT              \
    1, type },

#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }

typedef int (*inquiry)(PyObject *);  
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);
typedef void (*freefunc)(void *);

typedef struct PyModuleDef_Base {
  PyObject_HEAD3
  PyObject* (*m_init)(void);
  Py_ssize_t m_index;
  PyObject* m_copy;
} PyModuleDef_Base;
  
typedef struct PyModuleDef{
  PyModuleDef_Base m_base;
  const char* m_name;
  const char* m_doc;
  Py_ssize_t m_size;
  PyMethodDef *m_methods;
  inquiry m_reload;
  traverseproc m_traverse;
  inquiry m_clear;
  freefunc m_free;
}PyModuleDef;

typedef struct {
    int cf_flags;  /* bitmask of CO_xxx flags relevant to future */
} PyCompilerFlags;

struct _PyThreadState;
typedef struct _PyThreadState PyThreadState;

//struct _PyMethodDef;
//typedef struct _PyMethodDef PyMethodDef;

// Define some variables for type check
PyObject *R_PyNone;
PyObject *R_PyBool;
PyObject *R_PyTRUE;
PyObject *R_PyFALSE;
PyObject *R_PyInt;
PyObject *R_PyLong;
PyObject *R_PyFloat;
PyObject *R_PyString;
PyObject *R_PyUnicode;
PyObject *R_PyTuple;
PyObject *R_PyList;
PyObject *R_PyDict;

typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;
    
// Do some Python type definintion
typedef void (__cdecl *R_Py_Initialize) (void);
    R_Py_Initialize Py_Initialize;

typedef void (__cdecl *R_Py_InitializeEx) (int);
    R_Py_InitializeEx Py_InitializeEx;

#ifdef __x86_64__
    typedef PyObject * (__cdecl *R_Py_InitModule4_64) (const char *, PyMethodDef *, const char *, PyObject *, int);
    R_Py_InitModule4_64 Py_InitModule4_64;
    #define Py_InitModule4 Py_InitModule4_64
#else
    typedef PyObject * (__cdecl *R_Py_InitModule4) (const char *, PyMethodDef *, const char *, PyObject *, int);
        R_Py_InitModule4 Py_InitModule4; 
#endif
   
#define Py_InitModule(name, methods) \
        Py_InitModule4(name, methods, (char *)NULL, (PyObject *)NULL, \
                   PYTHON_API_VERSION)

typedef int (__cdecl *R_Py_Finalize) (void);
    R_Py_Finalize Py_Finalize;
 
typedef void (__cdecl *R_Py_SetPythonHome) (char *);
    R_Py_SetPythonHome Py_SetPythonHome; 

typedef int (__cdecl *R_Py_IsInitialized) (void);
    R_Py_IsInitialized Py_IsInitialized;

typedef int (__cdecl *R_PyArg_ParseTuple) (PyObject *, const char *, ...);
    R_PyArg_ParseTuple PyArg_ParseTuple;    
    

// Py_IncRef / Py_DecRef    
typedef void (__cdecl *R_Py_IncRef) (PyObject *);
    R_Py_IncRef Py_IncRef;

typedef void (__cdecl *R_Py_DecRef) (PyObject *);
    R_Py_DecRef Py_DecRef;

// PyErr
typedef void (__cdecl *R_PyErr_Print) (void);
    R_PyErr_Print PyErr_Print;	
	
// Py_Import
typedef PyObject * (__cdecl *R_PyImport_ImportModule) (const char *);
    R_PyImport_ImportModule PyImport_ImportModule;

typedef PyObject * (__cdecl *R_PyImport_AddModule) (const char *);
    R_PyImport_AddModule PyImport_AddModule;

typedef PyObject * (__cdecl *R_PyImport_ExecCodeModule) (const char *, PyObject *); // Py2 char * Py3 const char * different between Python 3
    R_PyImport_ExecCodeModule PyImport_ExecCodeModule;  

typedef PyObject * (__cdecl *R_PyImport_Import) (PyObject *);
    R_PyImport_Import PyImport_Import;

typedef int (__cdecl *R_PyImport_AppendInittab) (const char *, void (*initfunc)(void));
    R_PyImport_AppendInittab PyImport_AppendInittab;

typedef int (__cdecl *R_PyImport_AppendInittab3K) (const char *, PyObject* (*initfunc)(void));
    R_PyImport_AppendInittab3K PyImport_AppendInittab3K;

// Py_Run
typedef PyObject * (__cdecl *R_PyRun_StringFlags) (const char *, int, PyObject *, PyObject *, PyCompilerFlags *);
    R_PyRun_StringFlags PyRun_StringFlags;
#define PyRun_String(str, s, g, l) PyRun_StringFlags(str, s, g, l, NULL)

typedef int (__cdecl *R_PyRun_SimpleString) (const char *);
    R_PyRun_SimpleString PyRun_SimpleString;

typedef PyObject * (__cdecl *R_Py_BuildValue) (char *, ...);
    R_Py_BuildValue Py_BuildValue;

typedef PyObject * (__cdecl *R_PyObject_Str) (PyObject *);
    R_PyObject_Str PyObject_Str;

typedef PyObject * (__cdecl *R_PyObject_Bytes) (PyObject *);
    R_PyObject_Bytes PyObject_Bytes;
	
typedef PyObject * (__cdecl *R_PyObject_Unicode) (PyObject *);
    R_PyObject_Unicode PyObject_Unicode;

typedef PyObject * (__cdecl *R_PyObject_Repr) (PyObject *);
    R_PyObject_Repr PyObject_Repr;
    
// Py_Set
typedef void (__cdecl *R_Py_SetProgramName) (char *);
    R_Py_SetProgramName Py_SetProgramName;    

typedef void (__cdecl *R_Py_SetProgramName3K) (wchar_t *);
    R_Py_SetProgramName3K Py_SetProgramName3K;

// Py_Get
typedef const char * (__cdecl *R_Py_GetVersion) (void);
    R_Py_GetVersion Py_GetVersion;  

typedef char * (__cdecl *R_Py_GetPlatform) (void);
    R_Py_GetPlatform Py_GetPlatform;  

typedef char * (__cdecl *R_Py_GetCompiler) (void);
    R_Py_GetCompiler Py_GetCompiler; 

typedef char * (__cdecl *R_Py_GetBuildInfo) (void);
    R_Py_GetBuildInfo Py_GetBuildInfo;     

// PySys
typedef void (__cdecl *R_PySys_SetArgv) (int, char **);
    R_PySys_SetArgv PySys_SetArgv;
    
typedef void (__cdecl *R_PySys_SetArgv3K) (int, wchar_t **);
    R_PySys_SetArgv3K PySys_SetArgv3K;

typedef int (__cdecl *R_PySys_SetObject) (char *, PyObject *);
    R_PySys_SetObject PySys_SetObject;
    
// Py_Object
typedef PyObject * (__cdecl *R_PyObject_GetAttrString) (PyObject *, const char *); 
    R_PyObject_GetAttrString PyObject_GetAttrString;   

typedef int (__cdecl *R_PyObject_SetAttrString) (PyObject *, const char *, PyObject *);
    R_PyObject_SetAttrString PyObject_SetAttrString; 
    
typedef int (__cdecl *R_PyObject_HasAttrString) (PyObject *, const char *);
    R_PyObject_HasAttrString PyObject_HasAttrString; 
    
// Py_Module
typedef PyObject * (__cdecl *R_PyModule_GetDict) (PyObject *);
    R_PyModule_GetDict PyModule_GetDict;

typedef PyObject * (__cdecl *R_PyModule_Create2) (struct PyModuleDef*, int); // Python 3
    R_PyModule_Create2 PyModule_Create2;
#define PyModule_Create(module) \
	PyModule_Create2(module, PYTHON_API_VERSION)
    
// Py_Bool
typedef PyObject * (__cdecl *R_PyBool_FromLong) (long);
    R_PyBool_FromLong PyBool_FromLong;
    
// Py_Int
typedef PyObject * (__cdecl *R_PyInt_FromLong) (long);
    R_PyInt_FromLong PyInt_FromLong;

typedef PyObject * (__cdecl *R_PyInt_FromSsize_t) (Py_ssize_t);
    R_PyInt_FromSsize_t PyInt_FromSsize_t;
    
typedef long (__cdecl *R_PyInt_AsLong) (PyObject *);
    R_PyInt_AsLong PyInt_AsLong;

// Py_Long
typedef PyObject * (__cdecl *R_PyLong_FromLong) (long);
    R_PyLong_FromLong PyLong_FromLong;

typedef PyObject * (__cdecl *R_PyLong_FromSsize_t) (Py_ssize_t);
    R_PyLong_FromSsize_t PyLong_FromSsize_t;

typedef Py_ssize_t (__cdecl *R_PyLong_AsSsize_t) (PyObject *);
    R_PyLong_AsSsize_t PyLong_AsSsize_t;    
    
typedef long (__cdecl *R_PyLong_AsLong) (PyObject *);
    R_PyLong_AsLong PyLong_AsLong;
    
// Py_Float
typedef PyObject * (__cdecl *R_PyFloat_FromDouble) (double);
    R_PyFloat_FromDouble PyFloat_FromDouble;

typedef double (__cdecl *R_PyFloat_AsDouble) (PyObject *);
    R_PyFloat_AsDouble PyFloat_AsDouble;    

// PyBytes
typedef char * (__cdecl *R_PyBytes_AsString) (PyObject *);
    R_PyBytes_AsString PyBytes_AsString;

typedef PyObject * (__cdecl *R_PyBytes_FromString) (const char *);  // Python 3
    R_PyBytes_FromString PyBytes_FromString;
  
// Py_String    
typedef PyObject * (__cdecl *R_PyString_FromString) (const char *);
    R_PyString_FromString PyString_FromString;
    
typedef PyObject * (__cdecl *R_PyString_FromString23) (const char *);   // Python 2 & 3 dummy
    R_PyString_FromString23 PyString_FromString23;

typedef char * (__cdecl *R_PyString_AsString) (PyObject *);
    R_PyString_AsString PyString_AsString;
 
// Unicode
typedef PyObject * (__cdecl *R_PyUnicodeUCS2_FromString) (const char *);
    R_PyUnicodeUCS2_FromString PyUnicodeUCS2_FromString; 
    
typedef PyObject * (__cdecl *R_PyUnicode_FromString) (const char *);
    R_PyUnicode_FromString PyUnicode_FromString; 
//# define PyUnicode_FromString PyUnicodeUCS2_FromString  

typedef PyObject * (__cdecl *R_PyUnicodeUCS2_AsUTF8String) (PyObject *);
    R_PyUnicodeUCS2_AsUTF8String PyUnicodeUCS2_AsUTF8String;
    
typedef PyObject * (__cdecl *R_PyUnicodeUCS4_AsUTF8String) (PyObject *);
    R_PyUnicodeUCS4_AsUTF8String PyUnicodeUCS4_AsUTF8String;

typedef PyObject * (__cdecl *R_PyUnicode_AsUTF8String) (PyObject *);
    R_PyUnicode_AsUTF8String PyUnicode_AsUTF8String;

typedef PyObject * (__cdecl *R_PyUnicodeUCS2_FromFormat) (const char*, ...);
    R_PyUnicodeUCS2_FromFormat PyUnicodeUCS2_FromFormat;
    
typedef PyObject * (__cdecl *R_PyUnicodeUCS4_FromFormat) (const char*, ...);
    R_PyUnicodeUCS4_FromFormat PyUnicodeUCS4_FromFormat;
    
typedef PyObject * (__cdecl *R_PyUnicode_FromFormat) (const char*, ...);
    R_PyUnicode_FromFormat PyUnicode_FromFormat;
    
typedef PyObject * (__cdecl *R_PyInternalString_FromString) (const char *); // Python 2 and 3 since internal string is translated to PyUnicode instead of PyBytes one has to distingish between them
    R_PyInternalString_FromString PyInternalString_FromString;
    
// Py_Tuple
/* Macro, *only* to be used to fill in brand new tuples */
#define PyTuple_SET_ITEM(op, i, v) PyTuple_SetItem(op, i, v) //(((PyTupleObject *)(op))->ob_item[i] = v)
typedef PyObject * (__cdecl *R_PyTuple_New) (Py_ssize_t size);
    R_PyTuple_New PyTuple_New;

typedef PyObject * (__cdecl *R_PyTuple_GetItem) (PyObject *, Py_ssize_t);
    R_PyTuple_GetItem PyTuple_GetItem;

typedef int (__cdecl *R_PyTuple_SetItem) (PyObject *, Py_ssize_t, PyObject *);
    R_PyTuple_SetItem PyTuple_SetItem;

// Py_List
// int PyList_SetItem(PyObject *list, Py_ssize_t index, PyObject *item) // Maybe exchange through
#define PyList_SET_ITEM(op, i, v) PyList_SetItem(op, i, v) //(((PyListObject *)(op))->ob_item[i] = (v))
typedef int (__cdecl *R_PyList_Append) (PyObject *, PyObject *); 
    R_PyList_Append PyList_Append; 
    
typedef PyObject * (__cdecl *R_PyList_New) (Py_ssize_t size);
    R_PyList_New PyList_New;

typedef PyObject * (__cdecl *R_PyList_GetItem) (PyObject *, Py_ssize_t);
    R_PyList_GetItem PyList_GetItem;

typedef int (__cdecl *R_PyList_SetItem) (PyObject *, Py_ssize_t, PyObject *);
    R_PyList_SetItem PyList_SetItem;

// PyDict
typedef PyObject * (__cdecl *R_PyDict_New) (void);
    R_PyDict_New PyDict_New;
    
typedef PyObject * (__cdecl *R_PyDict_GetItem) (PyObject *, PyObject *);
    R_PyDict_GetItem PyDict_GetItem;

typedef int (__cdecl *R_PyDict_SetItem) (PyObject *, PyObject *, PyObject *);
    R_PyDict_SetItem PyDict_SetItem;

typedef PyObject * (__cdecl *R_PyDict_Keys) (PyObject *);
    R_PyDict_Keys PyDict_Keys;

typedef PyObject * (__cdecl *R_PyDict_Values) (PyObject *);
    R_PyDict_Values PyDict_Values;
    
typedef int (__cdecl *R_PyDict_Next) (PyObject *, Py_ssize_t *, PyObject **, PyObject **);
    R_PyDict_Next PyDict_Next;
 
typedef PyObject * (__cdecl *R_PyDict_GetItemString) (PyObject *, const char *);
    R_PyDict_GetItemString PyDict_GetItemString; 
    
typedef int (__cdecl *R_PyDict_DelItemString) (PyObject *, const char *);
    R_PyDict_DelItemString PyDict_DelItemString;
    
typedef int (__cdecl *R_PyDict_Merge) (PyObject *, PyObject *, int);
    R_PyDict_Merge PyDict_Merge;
    
typedef Py_ssize_t (__cdecl *R_PyDict_Size) (PyObject *);
    R_PyDict_Size PyDict_Size;

typedef void (__cdecl *R_PyDict_Clear) (PyObject *);
    R_PyDict_Clear PyDict_Clear;  
    
typedef int (__cdecl *R_PyDict_SetItemString) (PyObject *, const char *, PyObject *);
    R_PyDict_SetItemString PyDict_SetItemString;    
    
// Py_Mapping
//typedef int (__cdecl *R_PyMapping_SetItemString) (PyObject *, char *, PyObject *);
//    R_PyMapping_SetItemString PyMapping_SetItemString;

typedef int (__cdecl *R_PyMapping_SetItemString) (PyObject *, const char *, PyObject *); // Python 3
    R_PyMapping_SetItemString PyMapping_SetItemString;
    
//typedef int (__cdecl *R_PyMapping_SetItemString3K) (PyObject *, const char *, PyObject *); // Python 3
//    R_PyMapping_SetItemString3K PyMapping_SetItemString3K;
    
//typedef PyObject * (__cdecl *R_PyMapping_GetItemString) (PyObject *, char *);
    //R_PyMapping_GetItemString PyMapping_GetItemString;

typedef PyObject * (__cdecl *R_PyMapping_GetItemString) (PyObject *, const char *); // Python 3
    R_PyMapping_GetItemString PyMapping_GetItemString;
    
// typedef PyObject * (__cdecl *R_PyMapping_GetItemString3K) (PyObject *, const char *); // Python 3
    // R_PyMapping_GetItemString3K PyMapping_GetItemString3K;
    
// PyType
typedef int (__cdecl *R_PyType_IsSubtype) (PyObject *, PyObject *);
    R_PyType_IsSubtype PyType_IsSubtype;

// PyObject
typedef PyObject * (__cdecl *R_PyObject_Type) (PyObject *);
    R_PyObject_Type PyObject_Type;

typedef int (__cdecl *R_PyObject_IsInstance) (PyObject *, PyObject *);
    R_PyObject_IsInstance PyObject_IsInstance; 

typedef int (__cdecl *R_PyObject_IsTrue) (PyObject *);
    R_PyObject_IsTrue PyObject_IsTrue; 
    
typedef PyObject * (__cdecl *R_PyObject_Call) (PyObject *, PyObject *, PyObject *);
    R_PyObject_Call PyObject_Call; 

/* PyObject_CallMethod returns New reference
   NOTE: Python source has a note that the args get consumed in 
         call_function_tail therefore I must not decref it!!                                               */    
typedef PyObject * (__cdecl *R_PyObject_CallMethod) (PyObject *, char *, char *, ...);
    R_PyObject_CallMethod PyObject_CallMethod; 

typedef int (__cdecl *R_PyCallable_Check) (PyObject *);
    R_PyCallable_Check PyCallable_Check; 

// PyObject* PyObject_CallFunction(PyObject *callable, char *format, ...) Return value: New reference.    
typedef PyObject * (__cdecl *R_PyObject_CallFunction) (PyObject *, char *, ...);
    R_PyObject_CallFunction PyObject_CallFunction; 

#define PyNone_Check(o) (o == R_PyNone)
#define PyBool_Check(o) ((o == R_PyFALSE) | (o == R_PyTRUE))
#define PyInt_Check(o)  (Py_TYPE(o) == Py_TYPE(R_PyInt))
#define PyLong_Check(o)  (Py_TYPE(o) == Py_TYPE(R_PyLong))
#define PyFloat_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyFloat))
// These are implemented like the PyXXX_CheckExact
#define PyString_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyString))
#define PyUnicode_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyUnicode))
#define PyTuple_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyTuple))
#define PyList_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyList))
#define PyDict_Check(o) (Py_TYPE(o) == Py_TYPE(R_PyDict))

#define Py_REFCNT(ob)           (((PyObject*)(ob))->ob_refcnt)
#define Py_TYPE(ob)             (((PyObject*)(ob))->ob_type)
#define Py_SIZE(ob)             (((PyVarObject*)(ob))->ob_size)
#define PyTuple_GET_SIZE(op)    Py_SIZE(op)

// #define PyInt_AS_LONG(op) (((PyIntObject *)(op))->ob_ival)

#define PyObject_TypeCheck(ob, tp) \
    (Py_TYPE(ob) == (tp) || PyType_IsSubtype(Py_TYPE(ob), (tp)))
    
#define PyList_GET_SIZE(op)    Py_SIZE(op)

// #define PyBytes_AsString PyString_AsString des is a falsch!
                 
#define Py_RETURN_NONE return PY_NONE
// Py_INCREF is des gleiche wia Py_IncRef nur hoid ois object damit mas better getn kann
#define Py_INCREF(op) Py_IncRef(op)
#define Py_DECREF(op) Py_DecRef(op)
#define Py_XINCREF(op) do { if ((op) == NULL) ; else Py_IncRef(op); } while (0)
#define Py_XDECREF(op) do { if ((op) == NULL) ; else Py_DecRef(op); } while (0)

#define Py_CLEAR(op)                            \
    do {                                        \
        if (op) {                               \
            PyObject *_py_tmp = (PyObject *)(op);               \
            (op) = NULL;                        \
            Py_DECREF(_py_tmp);                 \
        }                                       \
    } while (0)
    
//#define PYMAPPING_GETITEMSTRING_R(dict, key) ((PYTHON_MAJOR_VERSION >= 3) ? PyMapping_GetItemString3K(dict, key) : PyMapping_GetItemString(dict, (char *)key))
#define PYMAPPING_GETITEMSTRING_R(dict, key) PyMapping_GetItemString(dict, (const char *)key)
    
//#define PYMAPPING_SETITEMSTRING_R(dict, key, value) ((PYTHON_MAJOR_VERSION >= 3) ? PyMapping_SetItemString3K(dict, key, value) : PyMapping_SetItemString(dict, (char *)key, value))
#define PYMAPPING_SETITEMSTRING_R(dict, key, value) PyMapping_SetItemString(dict, (const char *)key, value)

#define PY_TO_C_OBJECT_STRING(o) ((PYTHON_MAJOR_VERSION >= 3) ? PY_TO_C_UNICODE(o) : PY_TO_C_STRING(o))

#define PY_SET_PROGRAM_NAME(o) ((PYTHON_MAJOR_VERSION >= 3) ? Py_SetProgramName3K((wchar_t *)o) : Py_SetProgramName(o))

#define PyImport_APPENDINITTAB(c, o) ((PYTHON_MAJOR_VERSION >= 3) ? PyImport_AppendInittab3K(c, o) : PyImport_AppendInittab(c, o))

/*
// debug print
#define DP(txt) Rprintf("%s", txt)

// debug print ref count
#define DPRC(obj) Rprintf("#obj: %i", Py_REFCNT(obj))

#ifdef DEBUG
	#define DLOG(fmt, args...) Rprintf("%s:%d "fmt,__FILE__,__LINE__,args)
#else
	#define DLOG(fmt, args...)
#endif
*/

int get_proc_addresses(HMODULE);

#endif
#endif

