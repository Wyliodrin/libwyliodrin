
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

%pythoncode %{
import redis
import os
import json
import msgpack

port = 6379
channelClient = {}
threads = {}
handlers = {}
CHANNEL_SERVER = "communication_server:"
CHANNEL_CLIENT = "communication_client:"
client = None

def initCommunication (redis_port=6379):
  global port
  global client
  if redis_port is None:
    redis_port = os.getenv("wyliodrin_port", 6379);
  client = redis.StrictRedis(host='localhost', port=redis_port, db=0)
  port = redis_port
  return 0

def myHandlerFunction (message):
  global handlers
  global CHANNEL_SERVER

  channel = message['channel']
  label = channel[len(CHANNEL_CLIENT):]
  myHandler = handlers[label]

  if "wyliodrin_usemsgpack" in os.environ:
    mes = msgpack.unpackb(message['data'])
    fromId = mes['f']
    data = mes['d']
  else:
    mes = json.loads(message['data'])
    fromId = mes['from']
    data = mes['data']

  myHandler(fromId, label, 0, data)

def openConnection (label, handlerFunction):
  global channelClient
  global threads
  global handlers
  global CHANNEL_SERVER

  handlers[label] = handlerFunction
  r = redis.StrictRedis(host='localhost', port=port, db=0)
  p = r.pubsub(ignore_subscribe_messages=True)
  p.psubscribe(**{CHANNEL_CLIENT+str(label): myHandlerFunction})
  channelClient[label] = p
  thread = p.run_in_thread(sleep_time=0.01)

def sendMessage (wyliodrin_id, label, data):
  global client
  if isinstance(wyliodrin_id, list):
    for id in wyliodrin_id:
      message = {"id":id, "data":data}
      client.publish(CHANNEL_SERVER+str(label), json.dumps(message))
  else:
    message = {"id":wyliodrin_id, "data":data}
    client.publish(CHANNEL_SERVER+str(label), json.dumps(message))

def closeConnection(label):
  global threads
  global channelClient

  p = channelClient[label]
  if p is not None:
    p.close()
    del channelClient[label]

def closeCommunication():
  global threads
  global channelClient
  for c in channelClient:
    c.close()
%}


%include "../../src/wyliodrin.i"

