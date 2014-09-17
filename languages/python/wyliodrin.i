
%module(docstring="Python interface to libwyliodrin") wyliodrin

// This tells SWIG to treat char ** as a special case
%typemap(in) char ** {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (char **) malloc((size+1)*sizeof(char *));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyString_Check(o))
	$1[i] = PyString_AsString(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain strings");
	free($1);
	return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This cleans up the char ** array we malloc'd before the function call
%typemap(freearg) char ** {
  free((char *) $1);
}

// This tells SWIG to treat char ** as a special case
%typemap(in) double* {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (double*) malloc((size+1)*sizeof(double));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyFloat_Check(o))
	$1[i] = PyFloat_AsDouble(PyList_GetItem($input,i));
      else if (PyInt_Check(o))                      
        $1[i] = PyInt_AsLong(PyList_GetItem($input,i));
      else if (PyLong_Check(o))                      
        $1[i] = PyLong_AsLong(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain doubles");
	free($1);
	return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This tells SWIG to treat char ** as a special case
%typemap(in) unsigned int* {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (unsigned int*) malloc((size+1)*sizeof(unsigned int));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyFloat_Check(o))
  $1[i] = PyFloat_AsDouble(PyList_GetItem($input,i));
      else if (PyInt_Check(o))                      
        $1[i] = PyInt_AsLong(PyList_GetItem($input,i));
      else if (PyLong_Check(o))                      
        $1[i] = PyLong_AsLong(PyList_GetItem($input,i));
      else {
  PyErr_SetString(PyExc_TypeError,"list must contain doubles");
  free($1);
  return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This cleans up the char ** array we malloc'd before the function call
%typemap(freearg) double* {
  free((double *) $1);
}

%include "../../src/wyliodrin.i"
