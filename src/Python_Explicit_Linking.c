#ifdef PYTHON_IN_R_NO_EXPLICIT_LINKING
int ISO_C_forbids_an_empty_translation_unit = 0;
#else
#include "PythonInR.h"
#include "Python_Explicit_Linking.h"

R_Py_IsInitialized Py_IsInitialized = NULL;

int get_proc_addresses(HMODULE py_hdll){

    // Load 
    Py_Initialize = (R_Py_Initialize)GetProcAddress(py_hdll, "Py_Initialize");
        if (Py_Initialize == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_Initialize\n");
    
    Py_InitializeEx = (R_Py_InitializeEx)GetProcAddress(py_hdll, "Py_InitializeEx");
        if (Py_InitializeEx == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_InitializeEx\n");
    
    Py_SetPythonHome = (R_Py_SetPythonHome)GetProcAddress(py_hdll, "Py_SetPythonHome");
        if (Py_SetPythonHome == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_SetPythonHome\n");
        
    Py_Finalize = (R_Py_Finalize)GetProcAddress(py_hdll, "Py_Finalize");
        if (Py_Finalize == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_Finalize\n");
        
    Py_IsInitialized = (R_Py_IsInitialized)GetProcAddress(py_hdll, "Py_IsInitialized");
        if (Py_IsInitialized == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_IsInitialized\n");
        
    PyArg_ParseTuple = (R_PyArg_ParseTuple)GetProcAddress(py_hdll, "PyArg_ParseTuple");
        if (PyArg_ParseTuple == NULL) Rprintf("GetProcAddress: Warning couldn't load PyArg_ParseTuple\n");    
        
    // IncRef / DecRef
    Py_IncRef = (R_Py_IncRef)GetProcAddress(py_hdll, "Py_IncRef");
        if (Py_IncRef == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_IncRef\n");
    
    Py_DecRef = (R_Py_DecRef)GetProcAddress(py_hdll, "Py_DecRef");
        if (Py_DecRef == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_DecRef\n");
	
	// PyErr
	PyErr_Print = (R_PyErr_Print)GetProcAddress(py_hdll, "PyErr_Print");
        if (PyErr_Print == NULL) Rprintf("GetProcAddress: Warning couldn't load PyErr_Print\n");
	
    // Py_Import
	PyImport_ImportModule = NULL;
    PyImport_ImportModule = (R_PyImport_ImportModule)GetProcAddress(py_hdll, "PyImport_ImportModule");
        if (PyImport_ImportModule == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_ImportModule\n");
        
    PyImport_AddModule = (R_PyImport_AddModule)GetProcAddress(py_hdll, "PyImport_AddModule");
        if (PyImport_AddModule == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_AddModule\n");    
        
    PyImport_ExecCodeModule = (R_PyImport_ExecCodeModule)GetProcAddress(py_hdll, "PyImport_ExecCodeModule");
        if (PyImport_ExecCodeModule == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_ExecCodeModule\n");   
        
    PyImport_Import = (R_PyImport_Import)GetProcAddress(py_hdll, "PyImport_Import");
        if (PyImport_Import == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_Import\n");

    PyImport_AppendInittab = (R_PyImport_AppendInittab)GetProcAddress(py_hdll, "PyImport_AppendInittab");
        if (PyImport_AppendInittab == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_AppendInittab\n");    
        
    PyImport_AppendInittab3K = (R_PyImport_AppendInittab3K)GetProcAddress(py_hdll, "PyImport_AppendInittab");
        if (PyImport_AppendInittab3K == NULL) Rprintf("GetProcAddress: Warning couldn't load PyImport_AppendInittab3K\n");

    // Py_Run
    PyRun_StringFlags = (R_PyRun_StringFlags)GetProcAddress(py_hdll, "PyRun_StringFlags");
        if (PyRun_StringFlags == NULL) Rprintf("GetProcAddress: Warning couldn't load PyRun_StringFlags\n");
    
    PyRun_SimpleString = (R_PyRun_SimpleString)GetProcAddress(py_hdll, "PyRun_SimpleString");
        if (PyRun_SimpleString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyRun_SimpleString\n");
    
    Py_BuildValue = (R_Py_BuildValue)GetProcAddress(py_hdll, "Py_BuildValue");
        if (Py_BuildValue == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_BuildValue\n");
        
    PyObject_Str = (R_PyObject_Str)GetProcAddress(py_hdll, "PyObject_Str");
        if (PyObject_Str == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_Str\n");

    PyObject_Bytes = (R_PyObject_Bytes)GetProcAddress(py_hdll, "PyObject_Bytes");
        if ((PyObject_Bytes == NULL) & (PYTHON_MAJOR_VERSION > 2)) Rprintf("GetProcAddress: Warning couldn't load PyObject_Bytes\n");

    PyObject_Unicode = (R_PyObject_Unicode)GetProcAddress(py_hdll, "PyObject_Unicode");
        if ((PyObject_Unicode == NULL) & (PYTHON_MAJOR_VERSION < 3)) Rprintf("GetProcAddress: Warning couldn't load PyObject_Unicode\n");  		
		
	PyObject_Repr = (R_PyObject_Repr)GetProcAddress(py_hdll, "PyObject_Repr");
        if (PyObject_Repr == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_Repr\n");
	
    // Py_Get
    Py_GetVersion = (R_Py_GetVersion)GetProcAddress(py_hdll, "Py_GetVersion");
        if (Py_GetVersion == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_GetVersion\n");  
        
    Py_GetPlatform = (R_Py_GetPlatform)GetProcAddress(py_hdll, "Py_GetPlatform");
        if (Py_GetPlatform == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_GetPlatform\n");  

    Py_GetCompiler = (R_Py_GetCompiler)GetProcAddress(py_hdll, "Py_GetCompiler");
        if (Py_GetCompiler == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_GetCompiler\n");  

    Py_GetBuildInfo = (R_Py_GetBuildInfo)GetProcAddress(py_hdll, "Py_GetBuildInfo");
        if (Py_GetBuildInfo == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_GetBuildInfo\n");          

    // PySys
    PySys_SetArgv = (R_PySys_SetArgv)GetProcAddress(py_hdll, "PySys_SetArgv");
        if (PySys_SetArgv == NULL) Rprintf("GetProcAddress: Warning couldn't load PySys_SetArgv\n");  
    
    PySys_SetArgv3K = (R_PySys_SetArgv3K)GetProcAddress(py_hdll, "PySys_SetArgv");
        if (PySys_SetArgv3K == NULL) Rprintf("GetProcAddress: Warning couldn't load PySys_SetArgv3K\n");

    PySys_SetObject = (R_PySys_SetObject)GetProcAddress(py_hdll, "PySys_SetObject");
        if (PySys_SetObject == NULL) Rprintf("GetProcAddress: Warning couldn't load PySys_SetObject\n"); 
    
    // PyObject
    PyObject_GetAttrString = (R_PyObject_GetAttrString)GetProcAddress(py_hdll, "PyObject_GetAttrString");
        if (PyObject_GetAttrString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_GetAttrString\n"); 

    PyObject_SetAttrString = (R_PyObject_SetAttrString)GetProcAddress(py_hdll, "PyObject_SetAttrString");
        if (PyObject_SetAttrString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_SetAttrString\n"); 

    PyObject_HasAttrString = (R_PyObject_HasAttrString)GetProcAddress(py_hdll, "PyObject_HasAttrString");
        if (PyObject_HasAttrString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_HasAttrString\n"); 
        
    // Py_Module
    PyModule_GetDict = (R_PyModule_GetDict)GetProcAddress(py_hdll, "PyModule_GetDict");
        if (PyModule_GetDict == NULL) Rprintf("GetProcAddress: Warning couldn't load PyModule_GetDict\n");
    
    // Py_Bool
    PyBool_FromLong = (R_PyBool_FromLong)GetProcAddress(py_hdll, "PyBool_FromLong");
        if (PyBool_FromLong == NULL) Rprintf("GetProcAddress: Warning couldn't load PyBool_FromLong\n"); 
       
    // Py_Long
    PyLong_FromLong = (R_PyLong_FromLong)GetProcAddress(py_hdll, "PyLong_FromLong");
        if (PyLong_FromLong == NULL) Rprintf("GetProcAddress: Warning couldn't load PyLong_FromLong\n"); 
        
    PyLong_FromSsize_t = (R_PyLong_FromSsize_t)GetProcAddress(py_hdll, "PyLong_FromSsize_t");
        if (PyLong_FromSsize_t == NULL) Rprintf("GetProcAddress: Warning couldn't load PyLong_FromSsize_t\n"); 
        
    PyLong_AsSsize_t = (R_PyLong_AsSsize_t)GetProcAddress(py_hdll, "PyLong_AsSsize_t");
        if (PyLong_AsSsize_t == NULL) Rprintf("GetProcAddress: Warning couldn't load PyLong_AsSsize_t\n"); 
        
    PyLong_AsLong = (R_PyLong_AsLong)GetProcAddress(py_hdll, "PyLong_AsLong");
        if (PyLong_AsLong == NULL) Rprintf("GetProcAddress: Warning couldn't load PyLong_AsLong\n"); 
        
    // Py_Float
    PyFloat_FromDouble = (R_PyFloat_FromDouble)GetProcAddress(py_hdll, "PyFloat_FromDouble");
        if (PyFloat_FromDouble == NULL) Rprintf("GetProcAddress: Warning couldn't load PyFloat_FromDouble\n"); 
    
    PyFloat_AsDouble = (R_PyFloat_AsDouble)GetProcAddress(py_hdll, "PyFloat_AsDouble");
        if (PyFloat_AsDouble == NULL) Rprintf("GetProcAddress: Warning couldn't load PyFloat_AsDouble\n"); 
        
    // PyString
    // Here things get a little bit nasty since I don't know which version
    // I will get but luckily I can just try till I "guess" the right name.
    // In short I try to load PyUnicode_FromString if it is not available
    // I try to load PyUnicodeUCS2_FromString if this works I overload
    // PyUnicode_FromString with PyUnicodeUCS2_FromString but in the code
    // I use always PyUnicode_FromString.
    PyUnicode_FromString = (R_PyUnicode_FromString)GetProcAddress(py_hdll, "PyUnicode_FromString");
    if (PyUnicode_FromString == NULL){
        PyUnicodeUCS2_FromString = (R_PyUnicodeUCS2_FromString)GetProcAddress(py_hdll, "PyUnicodeUCS2_FromString");
        if (PyUnicodeUCS2_FromString == NULL){
            Rprintf("GetProcAddress: Warning couldn't load PyUnicode_FromString\n");
        }else{
            PyUnicode_FromString = (R_PyUnicode_FromString)PyUnicodeUCS2_FromString;
        }
    }
    
    PyUnicode_AsUTF8String = (R_PyUnicode_AsUTF8String)GetProcAddress(py_hdll, "PyUnicode_AsUTF8String");
    if (PyUnicode_AsUTF8String == NULL){
        PyUnicodeUCS2_AsUTF8String = (R_PyUnicodeUCS2_AsUTF8String)GetProcAddress(py_hdll, "PyUnicodeUCS2_AsUTF8String");
        if (PyUnicodeUCS2_AsUTF8String == NULL){ 
            PyUnicodeUCS4_AsUTF8String = (R_PyUnicodeUCS4_AsUTF8String)GetProcAddress(py_hdll, "PyUnicodeUCS4_AsUTF8String");
             if (PyUnicodeUCS4_AsUTF8String == NULL){
                Rprintf("GetProcAddress: Warning couldn't load PyUnicode_AsUTF8String\n");
             }else{
                PyUnicode_AsUTF8String = (R_PyUnicode_AsUTF8String)PyUnicodeUCS4_AsUTF8String;
             }
        }else{
            PyUnicode_AsUTF8String = (R_PyUnicode_AsUTF8String)PyUnicodeUCS2_AsUTF8String;
        }
    }
    
    // PyUnicode_FromFormat
    PyUnicode_FromFormat = (R_PyUnicode_FromFormat)GetProcAddress(py_hdll, "PyUnicode_FromFormat");
    if (PyUnicode_FromFormat == NULL){ // Try to load PyUnicodeUCS2_FromFormat
        PyUnicodeUCS2_FromFormat = (R_PyUnicodeUCS2_FromFormat)GetProcAddress(py_hdll, "PyUnicodeUCS2_FromFormat");
        if (PyUnicodeUCS2_FromFormat == NULL){ // Try to load PyUnicodeUCS4_FromFormat
            PyUnicodeUCS4_FromFormat = (R_PyUnicodeUCS4_FromFormat)GetProcAddress(py_hdll, "PyUnicodeUCS4_FromFormat");
            if (PyUnicodeUCS4_FromFormat == NULL){ 
                Rprintf("GetProcAddress: Warning couldn't load PyUnicode_FromFormat\n"); 
            }else{
                PyUnicode_FromFormat = (R_PyUnicode_FromFormat)PyUnicodeUCS4_FromFormat;
            }
        }else{
            PyUnicode_FromFormat = (R_PyUnicode_FromFormat)PyUnicodeUCS2_FromFormat;
        }
    }
    
    // Py_Tuple
    PyTuple_New = (R_PyTuple_New)GetProcAddress(py_hdll, "PyTuple_New");
        if (PyTuple_New == NULL) Rprintf("GetProcAddress: Warning couldn't load PyTuple_New\n"); 
    
    PyTuple_GetItem = (R_PyTuple_GetItem)GetProcAddress(py_hdll, "PyTuple_GetItem");
        if (PyTuple_GetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyTuple_GetItem\n"); 
        
    PyTuple_SetItem = (R_PyTuple_SetItem)GetProcAddress(py_hdll, "PyTuple_SetItem");
        if (PyTuple_SetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyTuple_SetItem\n"); 
        
    // PyList
    PyList_Append = (R_PyList_Append)GetProcAddress(py_hdll, "PyList_Append");
        if (PyList_Append == NULL) Rprintf("GetProcAddress: Warning couldn't load PyList_Append\n"); 
        
    PyList_New = (R_PyList_New)GetProcAddress(py_hdll, "PyList_New");
        if (PyList_New == NULL) Rprintf("GetProcAddress: Warning couldn't load PyList_New\n"); 
        
    PyList_GetItem = (R_PyList_GetItem)GetProcAddress(py_hdll, "PyList_GetItem");
        if (PyList_GetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyList_GetItem\n"); 
        
    PyList_SetItem = (R_PyList_SetItem)GetProcAddress(py_hdll, "PyList_SetItem");
        if (PyList_SetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyList_SetItem\n"); 
    
    // PyDict
    PyDict_New = (R_PyDict_New)GetProcAddress(py_hdll, "PyDict_New");
        if (PyDict_New == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_New\n");
    
    PyDict_GetItem = (R_PyDict_GetItem)GetProcAddress(py_hdll, "PyDict_GetItem");
        if (PyDict_GetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_GetItem\n");
        
    PyDict_SetItem = (R_PyDict_SetItem)GetProcAddress(py_hdll, "PyDict_SetItem");
        if (PyDict_SetItem == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_SetItem\n"); 
        
    PyDict_Keys = (R_PyDict_Keys)GetProcAddress(py_hdll, "PyDict_Keys");
        if (PyDict_Keys == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Keys\n"); 
        
    PyDict_Values = (R_PyDict_Values)GetProcAddress(py_hdll, "PyDict_Values");
        if (PyDict_Values == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Values\n");
        
    PyDict_Next = (R_PyDict_Next)GetProcAddress(py_hdll, "PyDict_Next");
        if (PyDict_Next == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Next\n");   
        
    PyDict_GetItemString = (R_PyDict_GetItemString)GetProcAddress(py_hdll, "PyDict_GetItemString");
        if (PyDict_GetItemString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_GetItemString\n");
          
    PyDict_DelItemString = (R_PyDict_DelItemString)GetProcAddress(py_hdll, "PyDict_DelItemString");
        if (PyDict_DelItemString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_DelItemString\n"); 
    
    PyDict_Merge = (R_PyDict_Merge)GetProcAddress(py_hdll, "PyDict_Merge");
        if (PyDict_Merge == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Merge\n"); 
        
    PyDict_Size = (R_PyDict_Size)GetProcAddress(py_hdll, "PyDict_Size");
        if (PyDict_Size == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Size\n"); 
    
    PyDict_Clear = (R_PyDict_Clear)GetProcAddress(py_hdll, "PyDict_Clear");
        if (PyDict_Clear == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_Clear\n"); 

    PyDict_SetItemString = (R_PyDict_SetItemString)GetProcAddress(py_hdll, "PyDict_SetItemString");
        if (PyDict_SetItemString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyDict_SetItemString\n"); 

    // PyType
    PyType_IsSubtype = (R_PyType_IsSubtype)GetProcAddress(py_hdll, "PyType_IsSubtype");
        if (PyType_IsSubtype == NULL) Rprintf("GetProcAddress: Warning couldn't load PyType_IsSubtype\n");    

    // PyObject
    PyObject_Type = (R_PyObject_Type)GetProcAddress(py_hdll, "PyObject_Type");
        if (PyObject_Type == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_Type\n");  
        
    PyObject_IsInstance = (R_PyObject_IsInstance)GetProcAddress(py_hdll, "PyObject_IsInstance");
        if (PyObject_IsInstance == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_IsInstance\n");  
        
    PyObject_IsTrue = (R_PyObject_IsTrue)GetProcAddress(py_hdll, "PyObject_IsTrue");
        if (PyObject_IsTrue == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_IsTrue\n");  
        
    PyObject_Call = (R_PyObject_Call)GetProcAddress(py_hdll, "PyObject_Call");
        if (PyObject_Call == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_Call\n");  
    
    PyObject_CallMethod = (R_PyObject_CallMethod)GetProcAddress(py_hdll, "PyObject_CallMethod");
        if (PyObject_CallMethod == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_CallMethod\n");  

    PyCallable_Check = (R_PyCallable_Check)GetProcAddress(py_hdll, "PyCallable_Check");
        if (PyCallable_Check == NULL) Rprintf("GetProcAddress: Warning couldn't load PyCallable_Check\n");  
    
    PyObject_CallFunction = (R_PyObject_CallFunction)GetProcAddress(py_hdll, "PyObject_CallFunction");
        if (PyObject_CallFunction == NULL) Rprintf("GetProcAddress: Warning couldn't load PyObject_CallFunction\n");
        
    // Py_Mapping    
    PyMapping_SetItemString = (R_PyMapping_SetItemString)GetProcAddress(py_hdll, "PyMapping_SetItemString");
        if (PyMapping_SetItemString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyMapping_SetItemString\n");    
        
    PyMapping_GetItemString = (R_PyMapping_GetItemString)GetProcAddress(py_hdll, "PyMapping_GetItemString");
        if (PyMapping_GetItemString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyMapping_GetItemString\n");
        
        
    if (PYTHON_MAJOR_VERSION >= 3){
        #ifdef __x86_64__
            Py_InitModule4_64 = NULL; // Replaced by PyModule_Create
        #endif
        
        PyModule_Create2 = (R_PyModule_Create2)GetProcAddress(py_hdll, "PyModule_Create2");
            if (PyModule_Create2 == NULL) Rprintf("GetProcAddress: Warning couldn't load PyModule_Create2\n");
        
        PyInt_FromLong = (R_PyInt_FromLong)PyLong_FromLong;
        PyInt_AsLong = (R_PyInt_AsLong)PyLong_AsLong; 
        PyInt_FromSsize_t = (R_PyInt_FromSsize_t)PyLong_FromSsize_t; 
		
		// PyBytes
		PyBytes_AsString = (R_PyBytes_AsString)GetProcAddress(py_hdll, "PyBytes_AsString");
			if (PyBytes_AsString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyBytes_AsString\n"); 
        
        PyString_AsString = (R_PyString_AsString)PyBytes_AsString;
        PyString_FromString = NULL;
        // I use this name to signal that eaven when normaly string is
        // changed to bytes in this case it was changed to unicode
        PyInternalString_FromString = (R_PyInternalString_FromString)PyUnicode_FromString;
		
		Py_SetProgramName3K = (R_Py_SetProgramName3K)GetProcAddress(py_hdll, "Py_SetProgramName");
			if (Py_SetProgramName3K == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_SetProgramName\n");   
        
    }else{
        PyModule_Create2 = NULL;
        
		PyString_FromString = (R_PyString_FromString)GetProcAddress(py_hdll, "PyString_FromString");
            if (PyString_FromString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyString_FromString\n"); 
        PyBytes_FromString = (R_PyBytes_FromString)PyString_FromString;
		PyInternalString_FromString = (R_PyInternalString_FromString)PyString_FromString;
		
		PyString_AsString = (R_PyString_AsString)GetProcAddress(py_hdll, "PyString_AsString");
            if (PyString_AsString == NULL) Rprintf("GetProcAddress: Warning couldn't load PyString_AsString\n"); 
		PyBytes_AsString = (R_PyBytes_AsString)PyString_AsString; // PyBytes_AsString only a MACRO in Python 2K
		
		Py_SetProgramName = (R_Py_SetProgramName)GetProcAddress(py_hdll, "Py_SetProgramName");
			if (Py_SetProgramName == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_SetProgramName\n");
        
        #ifdef __x86_64__
            Py_InitModule4_64 = (R_Py_InitModule4_64)GetProcAddress(py_hdll, "Py_InitModule4_64");
                if (Py_InitModule4_64 == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_InitModule4_64\n");  
        #else
            Py_InitModule4 = (R_Py_InitModule4)GetProcAddress(py_hdll, "Py_InitModule4");
                if (Py_InitModule4 == NULL) Rprintf("GetProcAddress: Warning couldn't load Py_InitModule4\n");
        #endif

        // Py_Int
        PyInt_FromLong = (R_PyInt_FromLong)GetProcAddress(py_hdll, "PyInt_FromLong");
            if (PyInt_FromLong == NULL) Rprintf("GetProcAddress: Warning couldn't load PyInt_FromLong\n");
    
        PyInt_FromSsize_t = (R_PyInt_FromSsize_t)GetProcAddress(py_hdll, "PyInt_FromSsize_t");
            if (PyInt_FromSsize_t == NULL) Rprintf("GetProcAddress: Warning couldn't load PyInt_FromSsize_t\n"); 
    
        PyInt_AsLong = (R_PyInt_AsLong)GetProcAddress(py_hdll, "PyInt_AsLong");
            if (PyInt_AsLong == NULL) Rprintf("GetProcAddress: Warning couldn't load PyInt_AsLong\n"); 
        
    }

    return 0;
}

#endif
