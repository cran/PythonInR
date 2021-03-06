#' # pyOptions
require(testthat)
require(PythonInR)
invisible(capture.output(pyConnect()))

## Options
expect_that(names(pyOptions()), 
	equals(c("numpyAlias", "useNumpy", "pandasAlias", "usePandas", "winPython364", "intToLong")))
expect_that(pyOptions("numpyAlias"), equals("numpy"))
pyOptions("numpyAlias", "np")
expect_that(pyOptions("numpyAlias"), equals("np"))
pyOptions("numpyAlias", "numpy")

