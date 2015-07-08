# ------------------------------------------------------------------------------ 
#
#   SetPythonObjects
#
# ------------------------------------------------------------------------------

#  ---------------------------------------------------------
#  pySet
#  =====
#' @title assigns R objects to Python
#'
#' @description The function pySet allows to assign R objects to the Python 
#'              namespace, the conversion from R to Python is done automatically.
#' @param key a string specifying the name of the Python object.
#' @param value a R object which is assigned to Python. 
#' @param namespace a string specifying where the key should be located.
#'                   If the namespace is set to "__main__" the key will be
#'                   set to the global namespace. But it is also possible to
#'                   set attributes of objects e.g. the attribute name of
#'                   the object 'os'.
#' @param useSetPoly an optional logical, giving if pySetPoly should be used 
#'                   to transform R objects into Python objects. For example if 
#'                   useSetPoly is TRUE unnamed vectors are transformed to 
#'                   Python objects of type PrVector else to lists.
#' @param useNumpy an optional logical, default is FALSE, to control if numpy 
#'                 should be used for the type conversion of matrices.
#' @param usePandas an optional logical, default is FALSE, to control if pandas 
#'                  should be used for the type conversion of data frames.
#' @details More information about the type conversion can be found in the README 
#'          file or at \url{http://pythoninr.bitbucket.org/}.
#' @examples
#' \dontshow{PythonInR:::pyCranConnect()}
#' pySet("x", 3)
#' pySet("M", diag(1,3))
#' pyImport("os")
#' pySet("name", "Hello os!", namespace="os")
#  ---------------------------------------------------------
pySet <- function(key, value, namespace = "__main__",
                  useSetPoly = TRUE,
                  useNumpy=pyOptions("useNumpy"),
                  usePandas=pyOptions("usePandas")){
    
    if ( pyConnectionCheck() ) return(invisible(NULL))
    check_string(key)

    if (useNumpy & (class(value) == "matrix")){
        class(value) <- "ndarray"
    }else if (usePandas & (class(value) == "data.frame")){
        class(value) <- "DataFrame"
    }
    
    if ( isBasic(value) | (!useSetPoly) ){
        returnValue <- pySetSimple(key, value, namespace)
    }else{
        returnValue <- pySetPoly(key, value, namespace)
    }
    invisible(returnValue)
}

# pySetSimple is a wrapper over the C function that users can
# ===========
# create new generic functions by using the function PythonInR:::pySetSimple
pySetSimple <- function(key, value, namespace="__main__"){
    .Call("r_set_py", namespace, key, value)
}

# pySetPoly is a polymorphic function
# =========
# The goal is to provide a part which can easily modified by the user. 
pySetPoly <- function(key, value, namespace="__main__"){
    pySetSimple(key, value, namespace)
}

setGeneric("pySetPoly")

# ----------------------------------------------------------
# vector
# ----------------------------------------------------------
pySetVector <- function(key, value){
    success <- pySetSimple(key, list(vector=unname(value), names=names(value), rClass=class(value)))    
    cmd <- sprintf("%s = __R__.PrVector(%s['vector'], %s['names'], %s['rClass'])", 
                   key, key, key, key)
    pyExec(cmd)
}

# logical
setMethod("pySetPoly", signature(key="character", value = "logical"),
          function(key, value) pySetVector(key, value))

# integer
setMethod("pySetPoly", signature(key="character", value = "integer"),
          function(key, value) pySetVector(key, value))

# numeric
setMethod("pySetPoly", signature(key="character", value = "numeric"),
          function(key, value) pySetVector(key, value))

# character
setMethod("pySetPoly", signature(key="character", value = "character"),
          function(key, value) pySetVector(key, value))

# ----------------------------------------------------------
# matrix
# ----------------------------------------------------------
# PrMatrix (a pretty reduced matrix class)
# ========
setMethod("pySetPoly", signature(key="character", value = "matrix"),
          function(key, value){
    rnam <- rownames(value)
    cnam <- colnames(value)
    xdim <- dim(value)
    rownames(value) <- NULL
    colnames(value) <- NULL
    value <- apply(value, 1, function(x) as.list(x))
    value <- list(matrix=value, rownames=rnam, colnames=cnam, dim=xdim)

    success <- PythonInR:::pySetSimple(key, value)

    cmd <- sprintf("%s = __R__.PrMatrix(%s['matrix'], %s['rownames'], %s['colnames'], %s['dim'])", 
                   key, key, key, key, key)
    pyExec(cmd)
})

# numpy.ndarray
# =============
setClass("ndarray")
setMethod("pySetPoly", signature(key="character", value = "ndarray"),
          function(key, value){
    rownames(value) <- NULL
    colnames(value) <- NULL
    value <- apply(value, 1, function(x) as.list(x))

    success <- pySetSimple(key, value)

    cmd <- sprintf("%s = %s.array(%s)",
                   key, pyOptions("numpyAlias"), key)
    pyExec(cmd)
})

# ----------------------------------------------------------
# data.frame
# ----------------------------------------------------------
# PrDataFrame
# ===========
setMethod("pySetPoly", signature(key="character", value = "data.frame"),
          function(key, value){
    rnam <- rownames(value)
    cnam <- colnames(value)
    xdim <- dim(value)
    rownames(value) <- NULL
    value <- list(data.frame=lapply(value, "["), rownames=rnam, colnames=cnam, dim=xdim)

    success <- pySetSimple(key, value)

    cmd <- sprintf("%s = __R__.PrDataFrame(%s['data.frame'], %s['rownames'], %s['colnames'], %s['dim'])", 
                   key, key, key, key, key)
    pyExec(cmd)
})

# pandas.DataFrame
# ================
setClass("DataFrame")
setMethod("pySetPoly", signature(key="character", value = "DataFrame"),
          function(key, value){
    rnam <- rownames(value)
    xdim <- dim(value)
    rownames(value) <- NULL
    value <- list(data.frame=lapply(value, "["), rownames=rnam)

    success <- pySetSimple(key, value)

    cmd <- sprintf("%s = %s.DataFrame(%s['data.frame'], index=%s['rownames'])",
                   key, pyOptions("pandasAlias"), key, key)
    pyExec(cmd)
})
