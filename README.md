# PythonInR - Makes accessing Python from within R as easy as pie.

## Dependencies

**Python** >= 2.7.0
**R** >= 2.15.0
 
**R-packages:**   
- pack


### Linux
**Python headers** 

On **Debian** and Debian-based Linux distributions (including **Ubuntu**
and other derivatives) the *"Python Development Headers"* can be installed
by typing the following into the terminal.

```bash
    apt-get install python-dev
```    

For installation on **Red Hat Enterprise Linux** , **Fedora**, and other **Red Hat
Linux-based** distributions, use the following:

```bash
    yum install python-devel
```

### Windows
There are no additional dependencies on Windows. 
(One obviously needs to have R and Python installed.)

## Installation
```r
    install.packages("PythonInR")
#   or
    install_bitbucket("Floooo/PythonInR")
```

## NOTES
### Python 3
Due to api changes in Python 3 the function `execfile` is no longer available.
The PythonInR package provides a `execfile` function following the typical
[workaround](http://www.diveintopython3.net/porting-code-to-python-3-with-2to3.html#execfile).
```python
def execfile(filename):
    exec(compile(open(filename, 'rb').read(), filename, 'exec'), globals())
```

## Type Casting
### R to Python (pySet)
To allow a nearly one to one conversion from R to Python, PythonInR provides
Python classes for vectors, matrices and data.frames which allow 
an easy conversion from R to Python and back. The names of the classes are PrVector,
PrMatrix and PrDataFrame.

#### Default Conversion
| R                  | length (n) | Python      |
| ------------------ | ---------- | ----------- |
| NULL               |            | None        |
| logical            |          1 | boolean     |
| integer            |          1 | integer     |
| numeric            |          1 | double      |
| character          |          1 | unicode     |
| logical            |      n > 1 | PrVector    |
| integer            |      n > 1 | PrVector    |
| numeric            |      n > 1 | PrVector    |
| character          |      n > 1 | PrVector    |
| list without names |      n > 0 | list        |
| list with names    |      n > 0 | dict        |
| matrix             |      n > 0 | PrMatrix    |
| data.frame         |      n > 0 | PrDataFrame |


#### Change the predefined conversion of pySet
PythonInR is designed in way that the conversion of types can easily be added or changed.
This is done by utilizing polymorphism: if pySet is called, pySet calls pySetPoly
which can be easily modified by the user. The following example shows how pySetPoly 
can be used to modify the behavior of pySet on the example of integer vectors.

The predefined type casting for integer vectors at an R level looks like the following:
```r
setMethod("pySetPoly", signature(key="character", value = "integer"),
          function(key, value){
    success <- pySetSimple(key, list(vector=unname(value), names=names(value), rClass=class(value)))
    cmd <- sprintf("%s = PythonInR.prVector(%s['vector'], %s['names'], %s['rClass'])", 
                   key, key, key, key)
    pyExec(cmd)
})
```
To change the predefined behavior one can simply use setMethod again.
```r
pySetPoly <- PythonInR:::pySetPoly
showMethods("pySetPoly")

pySet("x", 1:3)
pyPrint(x)
pyType("x")

setMethod("pySetPoly",
          signature(key="character", value = "integer"),
          function(key, value){
    PythonInR:::pySetSimple(key, value)
})

pySet("x", 1:3)
pyPrint(x)
pyType("x")
```
**NOTE PythonInR:::pySetSimple**   
The functions **pySetSimple** and **pySetPoly** shouldn't be used **outside** the function 
**pySet** since they do not check if R is connected to Python. If R is not connected 
to Python this can **yield** to **segfault** !


**NOTE (named lists):**   
When executing `pySet("x", list(b=3, a=2))` and `pyGet("x")` the order 
of the elements in x will change. This is not a special behavior of **PythonInR**
but the default behavior of Python for dictionaries.

**NOTE (matrix):**   
Matrices are either transformed to an object of the class PrMatrix or 
to an numpy array (if the option useNumpy is set to TRUE).


**NOTE (data.frame):**   
Data frames are either transformed to an object of the class PrDataFrame   
or to a pandas DataFrame (if the option usePandas is set to TRUE).


### R to Python (pyGet)
| Python      | R                    | simplify     |
| ----------- | -------------------- | ------------ |
| None        | NULL                 | TRUE / FALSE |
| boolean     | logical              | TRUE / FALSE |
| integer     | integer              | TRUE / FALSE |
| double      | numeric              | TRUE / FALSE |
| string      | character            | TRUE / FALSE |
| unicode     | character            | TRUE / FALSE |
| bytes       | character            | TRUE / FALSE |
| tuple       | list                 | FALSE        |
| tuple       | list or vector       | TRUE         |
| list        | list                 | FALSE        |
| list        | list or vector       | TRUE         |
| dict        | named list           | FALSE        |
| dict        | named list or vector | TRUE         |
| PrVetor     | vector               | TRUE / FALSE |
| PrMatrix    | matrix               | TRUE         |
| PrDataFrame | data.frame           | TRUE         |

#### Change the predefined conversion of pyGet
Similar to pySet the behavior of pyGet can be changed by utilizing pyGetPoly.
The predefined version of pyGetPoly for an object of class PrMatrix looks like the following:
```r
setMethod("pyGetPoly", signature(key="character", autoTypecast = "logical", simplify = "logical", pyClass = "PrMatrix"),
          function(key, autoTypecast, simplify, pyClass){
    x <- pyExecg(sprintf("x = %s.toDict()", key), autoTypecast = autoTypecast, simplify = simplify)[['x']]
    M <- do.call(rbind, x[['matrix']])
    rownames(M) <- x[['rownames']]
    colnames(M) <- x[['colnames']]
    return(M)
})
```
For objects of type "type" no conversion is defined. Therefore, PythonInR doesn't know how
to transform it into an R object so it will return a PythonInR_Object. This is kind of a
nice example since the return value of type(x) is a function therefore PythonInR will
return an object of type pyFunction.
```r
pyGet("type(list())")
```
One can define a new function to get elements of type "type" as follows.
```r
pyGetPoly <- PythonInR:::pyGetPoly
setClass("type")
setMethod("pyGetPoly", signature(key="character", autoTypecast = "logical", simplify = "logical", pyClass = "type"),
          function(key, autoTypecast, simplify, pyClass){
    pyExecg(sprintf("x = %s.__name__", key))[['x']]
})
pyGet("type(list())")
```

**NOTE pyGetPoly**   
The functions **pyGetPoly** should not be used **outside** the function 
**pyGet** since it does not check if R is connected to Python. If R is not connected 
to Python this will **yield** to **segfault** !


**NOTE (bytes):**   
In short, in Python 3 the data type string was replaced by the data type bytes.
More information can be found [here](http://www.diveintopython3.net/strings.html).


## Cheat Sheet

| Command          | Short Description                                  | Example Usage                                                        |
| ---------------- | -------------------------------------------------- | -------------------------------------------------------------------- |
| BEGIN.Python     | Start a Python read-eval-print loop                | `BEGIN.Python() print("Hello" + " " + "R!") END.Python`              |
| pyAttach         | Attach a Python object to an R environment         | `pyAttach("os.getcwd", .GlobalEnv)                                   |
| pyCall           | Call a callable Python object                      | `pyCall("pow", list(2,3), namespace="math")`                         |
| pyConnect        | Connect R to Python                                | `pyConnect()`                                                        |
| pyDict           | Create a representation of a Python dict in R      | `myNewDict <- pyDict('myNewDict', list(p=2, y=9, r=1))`              |
| pyDir            | The Python function dir (similar to ls)            | `pyDir()`                                                            |
| pyExec           | Execute Python code                                | `pyExec('some_python_code = "executed"')`                            |
| pyExecfile       | Execute a file (like source)                       | `pyExecfile("myPythonFile.py")`                                      |
| pyExecg          | Execute Python code and get all assigned variables | `pyExecg('some_python_code = "executed"')`                           |
| pyExecp          | Execute and print Python Code                      | `pyExecp('"Hello" + " " + "R!"')`                                    |
| pyExit           | Close Python                                       | `pyExit()`                                                           |
| pyFunction       | Create a representation of a Python function in R  | `pyFunction(key)`                                                    |
| pyGet            | Get a Python variable                              | `pyGet('myPythonVariable')`                                          |
| pyGet0           | Get a Python variable                              | `pyGet('myPythonVariable')`                                          |
| pyHelp           | Python help                                        | `pyHelp("help")`                                                     |
| pyImport         | Import a Python module                             | `pyImport("numpy", "np")`                                            |
| pyIsConnected    | Check if R is connected to Python                  | `pyIsConnected()`                                                    |
| pyList           | Create a representation of a Python list in R      | `pyList(key)`                                                        |
| pyObject         | Create a representation of a Python object in R    | `pyObject(key)`                                                      |
| pyOptions        | A function to get and set some package options     | `pyOptions("numpyAlias", "np")`                                      |
| pyPrint          | Print a Python variable from within R              | `pyPrint("somePythonVariable")`                                      |
| pySet            | Set a R variable in Python                         | `pySet("pi", pi)`                                                    |
| pySource         | A modified BEGIN.Python aware version of source    | `pySource("myFile.R")`                                               |
| pyTuple          | Create a representation of a Python tuple in R     | `pyTuple(key)`                                                       |
| pyType           | Get the type of a Python variable                  | `pyType("sys")`                                                      |
| pyVersion        | Returns the version of Python                      | `pyVersion()`                                                        |
