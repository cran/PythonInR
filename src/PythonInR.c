/* --------------------------------------------------------------------------  \

    PythonInR

    Should provide an stable easy to use interface from R to Python.

\  -------------------------------------------------------------------------- */
#include "PythonInR.h"

long pyrNamespaceCounter = 0;
// #TODO: ReThink if it would make sense to add Python like options!
// const char *unicode_errors = "replace"; 

#ifdef DEBUG_PYTHONINR
static FILE * log_file=NULL;
#endif

/*  ----------------------------------------------------------------------------
    log_write
      log_write captures the stderr of Python and prints it in the
      R stdout
  ----------------------------------------------------------------------------*/
PyObject* log_write(PyObject* self, PyObject* pArgs){
    char* LogStr = NULL;
    // Py_RETURN_NONE increfs Py_None (else segfault after 100 error messages :) )
    if (!PyArg_ParseTuple(pArgs, "s", &LogStr)) Py_RETURN_NONE; 

    if (strcmp(LogStr, "\n")) Rprintf("%s\n", LogStr);
    
    Py_RETURN_NONE;
}

PyObject* log_flush(PyObject* self, PyObject* args){
    Py_RETURN_NONE;
}

static PyMethodDef logMethods[] = {
            {"write", log_write, METH_VARARGS, "write stdout and stderr"},
            {"flush", log_flush, METH_VARARGS, "doc for flush"},
            {NULL, NULL, 0, NULL}
};

#if (PY_MAJOR_VERSION >= 3  || (!defined(PYTHON_IN_R_NO_EXPLICIT_LINKING)))
static struct PyModuleDef logMethodDef = {
        PyModuleDef_HEAD_INIT,
        "logCatcher",
        "This is a logging Module",
        -1,
        logMethods,
        NULL, NULL, NULL, NULL,
};

static PyObject* PyInit_logCatcher(void){
    PyObject* m = PyModule_Create(&logMethodDef);
    PySys_SetObject("stdout", m);
    PySys_SetObject("stderr", m);
    return m;
}
#endif


#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING

/*  ----------------------------------------------------------------------------
    py_connect
      py_connect creates a connection to Python
  ----------------------------------------------------------------------------*/
SEXP py_connect(SEXP initsigs){
    #if PY_MAJOR_VERSION >= 3
        static wchar_t *argv[1] = {L""};
        PyImport_AppendInittab("logCatcher", &PyInit_logCatcher);
    #else
        static char *argv[1] =  {""};
    #endif
    
    // http://www.informit.com/articles/article.aspx?p=22435
    #ifdef __unix__ /* __unix__ is usually defined by compilers targeting Unix systems */
        // if you don't load it _Py_ZeroStruct is undefined! Which leads to a complaint in
        // sys.execpthook! _Py_ZeroStruct is kind of the NULL (None) in Python
        // Py_None is actally a C macro wich returns _Py_ZeroStruct
        // Loading the library removes the sys.execpthook error. (In Python 3)
        dlopen( xstr(PYTHONLIBXY), RTLD_NOW | RTLD_GLOBAL ); // NOTE: use RTLD_NOW for debugging RTLD_LAZY else
    #endif
    
    Py_InitializeEx(asInteger(initsigs));
    PySys_SetArgv(1, argv);
    Py_SetProgramName(PY_V_CHAR("PythonInR")); 
    
    PyRun_SimpleString("import sys; sys.path.append('.')");

    #if PY_MAJOR_VERSION < 3
        PyObject *m = Py_InitModule("logCatcher", logMethods);
        if (m == NULL) error("Couldn't initialize the logCatcher");
        PySys_SetObject("stdout", m);
        PySys_SetObject("stderr", m);
        PyRun_SimpleString("import logCatcher\n"
                           "import sys");
    #else
        PyImport_ImportModule("logCatcher");
        // import define a alternative to execfile as sugested at various sources
        // http://www.diveintopython3.net/porting-code-to-python-3-with-2to3.html
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("def execfile(filename):\n    exec(compile(open(filename, 'rb').read(), filename, 'exec'), globals())");
    #endif
    
    return R_NilValue; 
}

SEXP isDllVersion(void){
    return c_to_r_integer(0);
}

/*  ----------------------------------------------------------------------------
    py_close
      py_close closes the python connection
  ----------------------------------------------------------------------------*/
SEXP py_close(void){
    Py_Finalize();
    return R_NilValue;
}

/*  ----------------------------------------------------------------------------
    py_is_connected
      py_is_connected checks if R is connected to Python
       
      Returns true (one) when the Python interpreter has been initialized, 
      false (zero) if not. After Py_Finalize() is called, this returns false 
      until Py_Initialize() is called again.
  ----------------------------------------------------------------------------*/
SEXP py_is_connected(void){
    return c_to_r_integer(Py_IsInitialized());
}

#else

/*  ----------------------------------------------------------------------------

    Windows Version
    
  ----------------------------------------------------------------------------*/
SEXP py_set_major_version(SEXP pythonMajorVersion){
    PYTHON_MAJOR_VERSION = asInteger(pythonMajorVersion);
    return R_NilValue;
}

SEXP py_connect(SEXP dllName, SEXP dllDir, SEXP alteredSearchPath){
    #ifdef DEBUG_PYTHONINR
        log_file=fopen("PythonInR.log","w");
    #endif
    logging("WIN py_connect: CONNECT to dll!");
    const char *dll_name = CHAR(STRING_ELT(dllName, 0)); // its possible that  if a full path where provided non ascii character would go wrong!
    
    if (GET_LENGTH(dllName) == 0) error("no dllName provided!");
    if (GET_LENGTH(dllDir) != 0){
        const char *dll_dir = CHAR(STRING_ELT(dllDir, 0));
        logging("WIN py_connect: Set dll directory to: %s\n", dll_dir);
        SetDllDirectory(dll_dir);
    }
    if (GET_LENGTH(alteredSearchPath) == 0){
        logging("WIN py_connect: Load library: %s\n", dll_name);
        py_hdll = LoadLibraryA(dll_name);
    }else{
        logging("WIN py_connect: Load library with altered search path: %s\n", dll_name);
        /* I use LoadLibraryEx so Windows looks for dependent DLLs
           in directory of pathname first. */
        py_hdll = LoadLibraryEx(dll_name, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
    if (py_hdll == NULL) error("Error couldn't load dll file!\n");
    return R_NilValue;
}

SEXP py_free_dll(void){
    FreeLibrary(py_hdll);
    return R_NilValue;
}

/* py_initialize initializes python*/
SEXP py_initialize(SEXP initsigs){
    logging("py_initialize: Start initialization!");
    static wchar_t *argv3K[1] = {L""};
    static char *argv2K[1] =  {""};
    
    Py_InitializeEx(asInteger(initsigs));
    
    logging("py_initialize: SetArgv!");
    if (PYTHON_MAJOR_VERSION >= 3) PySys_SetArgv3K(1, argv3K);
    else PySys_SetArgv(1, argv2K);
    
    logging("py_initialize: Append to path!");
    PyRun_SimpleString("import sys; sys.path.append('.')");
    
    PYTHON_API_VERSION = py_get_api_version(); // PyModule_Create needs the API_VERSION!
    
    return R_NilValue; 
}

int py_get_api_version(void){
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *py_namespace_dict = PyModule_GetDict(sys);
    Py_XINCREF(py_namespace_dict);
    const char* key = "api_version";
    PyObject *py_object = PYMAPPING_GETITEMSTRING_R(py_namespace_dict, key);
    if (py_object == NULL){
            PyRun_SimpleString("\n");
            error("couldn't get api_version");
    }
    int api_version = (int)PY_TO_C_INTEGER(py_object);
    Py_XDECREF(sys);
    Py_XDECREF(py_namespace_dict);
    Py_XDECREF(py_object);
    return api_version;
}

SEXP py_import_append_logCatcher(void){
    if(PYTHON_MAJOR_VERSION >= 3){
        PyImport_AppendInittab3K("logCatcher", &PyInit_logCatcher);
        return(c_to_r_integer(0));
    }
    return c_to_r_integer(-1);
}

SEXP py_init_redirect_stderrout(void){
    if (PYTHON_MAJOR_VERSION == 2){
        PyObject *m = Py_InitModule("logCatcher", logMethods);
        if (m == NULL) error("Couldn't initialize the logCatcher");
        PySys_SetObject("stdout", m);
        PySys_SetObject("stderr", m);
        PyRun_SimpleString("import logCatcher\n"
                           "import sys");
    }else if (PYTHON_MAJOR_VERSION >= 3){
        const char* moduleName = "logCatcher";
        PyImport_ImportModule(moduleName);
    }
    return R_NilValue;
}

/* normally this is is all provided by the header*/
SEXP py_init_py_values(void){
    // Rprintf("Start initialization ...\n");
    // Some py values
    R_PyNone = Py_BuildValue("z", NULL);
    R_PyBool = PyBool_FromLong(1);
    R_PyTRUE = PyBool_FromLong(0L);
    R_PyFALSE = PyBool_FromLong(1L);
    R_PyInt = PyInt_FromLong(3L);
    R_PyLong = PyLong_FromLong(1);
    R_PyFloat = PyFloat_FromDouble(1);
    /*
      Python 3’s str() type is equivalent to Python 2’s unicode(); the C functions are called
      PyUnicode_* for both. The old 8-bit string type has become bytes(), with C functions
      called PyBytes_*. 
    */
    if (PYTHON_MAJOR_VERSION >= 3){
        R_PyString = Py_BuildValue("y", "a");
    }else{
        R_PyString = Py_BuildValue("s", "a");
    }
    R_PyUnicode = Py_BuildValue("u", "a");
    R_PyTuple = Py_BuildValue("(i)", 1);
    R_PyList = Py_BuildValue("[i]", 1);
    R_PyDict = Py_BuildValue("{s:i}", "a", 1);
    // Rprintf("Finished initialization!\n");
    return R_NilValue;
}

SEXP isDllVersion(void){
    return c_to_r_integer(1);
}

SEXP py_get_process_addresses(void){
    logging("py_get_process_addresses: Start loading process addresses for Python %i ...", PYTHON_MAJOR_VERSION);
    get_proc_addresses(py_hdll);
    logging("py_get_process_addresses: Finished loading process addresses!");
    return R_NilValue;
}

SEXP py_set_program_name(SEXP programName){
    logging("py_set_program_name: Set Program Name!");
    const char *python_home = R_TO_C_STRING(programName);
    if (PYTHON_MAJOR_VERSION >= 3){
        Py_SetProgramName3K(((wchar_t *)python_home));
    }else{
        Py_SetProgramName((char *)python_home);
    }
    return R_NilValue;
}

SEXP py_set_home(SEXP pythonHome){
    const char *python_home = R_TO_C_STRING(pythonHome);
    Py_SetPythonHome((char *)python_home);
    return R_NilValue;
}

/*  ----------------------------------------------------------------------------
    py_is_connected
      py_is_connected checks if R is connected to Python
       
      Returns true (one) when the Python interpreter has been initialized, 
      false (zero) if not. After Py_Finalize() is called, this returns false 
      until Py_Initialize() is called again.
  ----------------------------------------------------------------------------*/
SEXP py_is_connected(void){
    if (Py_IsInitialized == NULL) return c_to_r_integer(0);
    return c_to_r_integer(Py_IsInitialized());
}

/*  ----------------------------------------------------------------------------
    py_close
      py_close closes the python connection
  ----------------------------------------------------------------------------*/
SEXP py_close(void){
    Py_Finalize();
    FreeLibrary(py_hdll);
    Py_IsInitialized = NULL;
    return R_NilValue;
}

#endif

/*  ----------------------------------------------------------------------------
    py_get_info
      py_get_info gives you some information about the Python distribution you 
      are connected to like:
        -) the platform
        -) the compiler used to compile python (which can be important due 
           compatibility issues with Microsoft Visual Studio)
        -) the build info
  ----------------------------------------------------------------------------*/
SEXP py_get_info(void){
    SEXP info;
    PROTECT(info = allocVector(STRSXP, 3));
    SET_STRING_ELT(info, 0, mkChar(Py_GetPlatform()));
    SET_STRING_ELT(info, 1, mkChar(Py_GetCompiler()));
    SET_STRING_ELT(info, 2, mkChar(Py_GetBuildInfo()));
    UNPROTECT(1);
    return info;
}
