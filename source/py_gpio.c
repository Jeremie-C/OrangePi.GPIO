/*
Copyright (c) 2012-2016 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "c_gpio.h"
#include "event_gpio.h"
#include "py_pwm.h"
#include "boards.h"
#include "common.h"
#include "constants.h"

static int gpio_warnings = 1;

struct py_callback
{
  unsigned int gpio;
  PyObject *py_cb;
  struct py_callback *next;
};
static struct py_callback *py_callbacks = NULL;

static int mmap_gpio_mem(void)
{
  int result;

  if (module_setup)
    return 0;

  result = setup();
  if (result == SETUP_DEVMEM_FAIL)
  {
    PyErr_SetString(PyExc_RuntimeError, "No access to /dev/mem.  Try running as root!");
    return 1;
  } else if (result == SETUP_MALLOC_FAIL) {
    PyErr_NoMemory();
    return 2;
  } else if (result == SETUP_MMAP_FAIL) {
    PyErr_SetString(PyExc_RuntimeError, "Mmap of GPIO registers failed");
    return 3;
  } else { // result == SETUP_OK
    module_setup = 1;
		//printf("SETUP_OK!\n");
    return 0;
  }
}

// python function setboard(board)
static PyObject *py_setopi(PyObject *self, PyObject *args)
{
  if (!PyArg_ParseTuple(args, "i", &board_type))
    return NULL;

  if (setup_error)
  {
    PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
    return NULL;
  }

  if (board_type < ZERO || board_type > PRIME )
  {
    PyErr_SetString(PyExc_ValueError, "An invalid board was passed to setboard()");
    return NULL;
  }

  //here is the 'pin_to_gpio' initialization
  switch (board_type) {
    case 1 :
    case 2 : pin_to_gpio = &pin_to_gpio_zero; break;
    case 3 : pin_to_gpio = &pin_to_gpio_zero2; break;
    case 4 : pin_to_gpio = &pin_to_gpio_pc; break;
    case 5 : pin_to_gpio = &pin_to_gpio_pc2; break;
    case 6 : pin_to_gpio = &pin_to_gpio_prime; break;
  }

  Py_RETURN_NONE;
}

static PyObject *py_getopi(PyObject *self, PyObject *args)
{
   PyObject *value;

   if (setup_error)
   {
      PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
      return NULL;
   }

   if (board_type == BOARD_UNKNOWN)
      Py_RETURN_NONE;

   value = Py_BuildValue("i", board_type);
   return value;
}

// python function setmode(mode)
static PyObject *py_setmode(PyObject *self, PyObject *args)
{
  if (!PyArg_ParseTuple(args, "i", &gpio_mode))
    return NULL;

  if (setup_error)
  {
      PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
      return NULL;
  }

  if (gpio_mode != BOARD && gpio_mode != BCM && gpio_mode != MODE_SOC)
  {
    PyErr_SetString(PyExc_ValueError, "An invalid mode was passed to setmode()");
    return NULL;
  }

  if (gpio_mode != MODE_SOC && board_type == BOARD_UNKNOWN)
  {
    PyErr_SetString(PyExc_ValueError, "Please use setboard(board) before setmode()");
    return NULL;
  }

  Py_RETURN_NONE;
}

static PyObject *py_getmode(PyObject *self, PyObject *args)
{
   PyObject *value;

   if (setup_error)
   {
      PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
      return NULL;
   }

   if (gpio_mode == MODE_UNKNOWN)
      Py_RETURN_NONE;

   value = Py_BuildValue("i", gpio_mode);
   return value;
}

// python function setwarnings(state)
static PyObject *py_setwarnings(PyObject *self, PyObject *args)
{
   if (!PyArg_ParseTuple(args, "i", &gpio_warnings))
      return NULL;

   if (setup_error)
   {
      PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
      return NULL;
   }

   Py_RETURN_NONE;
}

// python function setup(channel, direction, pull_up_down=PUD_OFF, initial=None)
static PyObject *py_setup_channel(PyObject *self, PyObject *args, PyObject *kwargs)
{
  unsigned int gpio;
  int channel, direction;
  int pud = PUD_OFF + PY_PUD_CONST_OFFSET;
  int initial = -1;
  static char *kwlist[] = {"channel", "direction", "pull_up_down", "initial", NULL};
  int func, fn;

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|ii", kwlist, &channel, &direction, &pud, &initial))
    return NULL;

  // check module has been imported cleanly
  if (setup_error)
  {
    PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
    return NULL;
  }

  if (get_gpio_number(channel, &gpio))
    return NULL;

  if (direction != INPUT && direction != OUTPUT)
  {
    PyErr_SetString(PyExc_ValueError, "An invalid direction was passed to setup()");
    return NULL;
  }

  if (direction == OUTPUT)
    pud = PUD_OFF + PY_PUD_CONST_OFFSET;

  pud -= PY_PUD_CONST_OFFSET;
  if (pud != PUD_OFF && pud != PUD_DOWN && pud != PUD_UP)
  {
    PyErr_SetString(PyExc_ValueError, "Invalid value for pull_up_down - should be either PUD_OFF, PUD_UP or PUD_DOWN");
    return NULL;
  }

  if (mmap_gpio_mem())
    return NULL;

  func = gpio_function(gpio);
  /* Prevent Break of SPI, TWI, UART,... */
  if (func != 0 && func != 1 && func != 7) {
    fn = gpio_function_name(gpio, func, board_type);
    PyErr_Format(PyExc_ValueError, "This channel is already in use by system as %s.", FUNCTIONS[fn]);
    return NULL;
  }

  if (gpio_warnings && (gpio_direction[gpio] == -1 && func == 1))
  {
    PyErr_Warn(NULL, "This channel is already in use, continuing anyway. Use GPIO.setwarnings(False) to disable warnings.");
  }

  if (direction == OUTPUT && (initial == LOW || initial == HIGH))
  {
    output_gpio(gpio, initial);
  }
  setup_gpio(gpio, direction, pud);
  gpio_direction[gpio] = direction;

  Py_RETURN_NONE;
}

// python function output(channel, value)
static PyObject *py_output_gpio(PyObject *self, PyObject *args)
{
   unsigned int gpio;
   int channel, value;

   if (!PyArg_ParseTuple(args, "ii", &channel, &value))
      return NULL;

   if (get_gpio_number(channel, &gpio))
       return NULL;

   if (gpio_direction[gpio] != OUTPUT)
   {
      PyErr_SetString(PyExc_RuntimeError, "The GPIO channel has not been set up as an OUTPUT");
      return NULL;
   }

   if (check_gpio_priv())
      return NULL;

   output_gpio(gpio, value);
   Py_RETURN_NONE;
}

// python function value = input(channel)
static PyObject *py_input_gpio(PyObject *self, PyObject *args)
{
   unsigned int gpio;
   int channel;
   PyObject *value;

   if (!PyArg_ParseTuple(args, "i", &channel))
      return NULL;

   if (get_gpio_number(channel, &gpio))
       return NULL;

   // check channel is set up as an input or output
   if (gpio_direction[gpio] != INPUT && gpio_direction[gpio] != OUTPUT)
   {
      PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel first");
      return NULL;
   }

   if (check_gpio_priv())
      return NULL;

   if (input_gpio(gpio)) {
      value = Py_BuildValue("i", HIGH);
   } else {
      value = Py_BuildValue("i", LOW);
   }
   return value;
}

// python function value = gpio_function(channel)
static PyObject *py_gpio_function(PyObject *self, PyObject *args)
{
  unsigned int gpio;
  int channel;
  int f;
  int fn;
  PyObject *func;

  if (!PyArg_ParseTuple(args, "i", &channel))
    return NULL;

  if (get_gpio_number(channel, &gpio))
    return NULL;

  if (mmap_gpio_mem())
    return NULL;

  f = gpio_function(gpio);

  switch (f)
  {
    case 0 : fn = INPUT;  break;
    case 1 : fn = OUTPUT; break;
    case 2 :
    case 3 :
    case 4 :
    case 5 :
    case 6 :
    case 7 : fn = f; break;
    default : fn = ALT_UNKNOWN; break;
  }

  func = Py_BuildValue("i", fn);
  return func;
}

// python function value = gpio_function_name(channel)
static PyObject *py_gpio_function_string(PyObject *self, PyObject *args)
{
  unsigned int gpio;
  int channel;
  int f;
  int fn;
  char *str;
  PyObject *func;

  if (!PyArg_ParseTuple(args, "i", &channel))
    return NULL;

  if (get_gpio_number(channel, &gpio))
    return NULL;

  if (mmap_gpio_mem())
    return NULL;

  f = gpio_function(gpio);

  fn = gpio_function_name(gpio,f,board_type);

  str = FUNCTIONS[fn];

  func = Py_BuildValue("s", str);
  return func;
}

// python function cleanup(channel=None)
static PyObject *py_cleanup(PyObject *self, PyObject *args, PyObject *kwargs)
{
	int i;
	int found = 0;
	int channel = -666;
	unsigned int gpio;
	static char *kwlist[] = {"channel", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &channel))
		return NULL;

	if (channel != -666 && get_gpio_number(channel, &gpio))
		return NULL;

  if (module_setup && !setup_error) {
    if (channel == -666) {
      // clean up any /sys/class exports
      event_cleanup_all();
      // set everything back to input
      for (i=0; i<383; i++) {
        if (gpio_direction[i] != -1) {
          //printf("Clean %d\n",i);
          setup_gpio(i, INPUT, PUD_OFF); //take care
          gpio_direction[i] = -1;
          found = 1;
          //printf("Clean Fnished %d \n",i);
        }
      }
    } else {
      // clean up any /sys/class exports
      event_cleanup(gpio);
      // set everything back to input
      if (gpio_direction[gpio] != -1) {
        setup_gpio(gpio, INPUT, PUD_OFF);
        gpio_direction[gpio] = -1;
        found = 1;
      }
    }
  }

  if (!found && gpio_warnings) {
   PyErr_Warn(NULL, "No channels have been set up yet - nothing to clean up!  Try cleaning up at the end of your program instead!");
  }

  Py_RETURN_NONE;
}

/** EVENT **/
static void run_py_callbacks(unsigned int gpio)
{
  PyObject *result;
  PyGILState_STATE gstate;
  struct py_callback *cb = py_callbacks;

  while (cb != NULL)
  {
    if (cb->gpio == gpio) {
      // run callback
      gstate = PyGILState_Ensure();
      result = PyObject_CallFunction(cb->py_cb, "i", gpio);
      if (result == NULL && PyErr_Occurred()){
        PyErr_Print();
        PyErr_Clear();
      }
      Py_XDECREF(result);
      PyGILState_Release(gstate);
    }
    cb = cb->next;
  }
}

static int add_py_callback(unsigned int gpio, PyObject *cb_func)
{
  struct py_callback *new_py_cb;
  struct py_callback *cb = py_callbacks;

  // add callback to py_callbacks list
  new_py_cb = malloc(sizeof(struct py_callback));
  if (new_py_cb == 0)
  {
    PyErr_NoMemory();
    return -1;
  }
  new_py_cb->py_cb = cb_func;
  Py_XINCREF(cb_func);         // Add a reference to new callback
  new_py_cb->gpio = gpio;
  new_py_cb->next = NULL;
  if (py_callbacks == NULL) {
    py_callbacks = new_py_cb;
  } else {
    // add to end of list
    while (cb->next != NULL)
      cb = cb->next;

    cb->next = new_py_cb;
  }
  add_edge_callback(gpio, run_py_callbacks);
  return 0;
}

// python function add_event_callback(gpio, callback)
static PyObject *py_add_event_callback(PyObject *self, PyObject *args, PyObject *kwargs)
{
  unsigned int gpio;
  int channel;
  PyObject *cb_func;
  char *kwlist[] = {"gpio", "callback", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iO|i", kwlist, &channel, &cb_func))
    return NULL;

  if (!PyCallable_Check(cb_func))
  {
    PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
    return NULL;
  }

  if (get_gpio_number(channel, &gpio))
    return NULL;

  // check channel is set up as an input
  if (gpio_direction[gpio] != INPUT)
  {
    PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
    return NULL;
  }

  if (!gpio_event_added(gpio))
  {
    PyErr_SetString(PyExc_RuntimeError, "Add event detection using add_event_detect first before adding a callback");
    return NULL;
  }

  if (add_py_callback(gpio, cb_func) != 0)
    return NULL;

  Py_RETURN_NONE;
}

// python function add_event_detect(gpio, edge, callback=None, bouncetime=0
static PyObject *py_add_event_detect(PyObject *self, PyObject *args, PyObject *kwargs)
{
  unsigned int gpio;
  int channel, edge, result;
  unsigned int bouncetime = 0;
  PyObject *cb_func = NULL;
  char *kwlist[] = {"gpio", "edge", "callback", "bouncetime", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|Oi", kwlist, &channel, &edge, &cb_func, &bouncetime))
    return NULL;

  if (cb_func != NULL && !PyCallable_Check(cb_func))
  {
    PyErr_SetString(PyExc_TypeError, "Parameter must be callable");
    return NULL;
  }

	if (get_gpio_number(channel, &gpio))
    return NULL;

  // check channel is set up as an input
  if (gpio_direction[gpio] != INPUT)
  {
    PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
    return NULL;
  }
  // is edge valid value
  edge -= PY_EVENT_CONST_OFFSET;
  if (edge != RISING_EDGE && edge != FALLING_EDGE && edge != BOTH_EDGE)
  {
    PyErr_SetString(PyExc_ValueError, "The edge must be set to RISING, FALLING or BOTH");
    return NULL;
  }

  if (check_gpio_priv())
    return NULL;

  if ((result = add_edge_detect(gpio, edge, bouncetime)) != 0)   // starts a thread
  {
    if (result == 1)
    {
      PyErr_SetString(PyExc_RuntimeError, "Edge detection already enabled for this GPIO channel");
      return NULL;
    } else {
      PyErr_SetString(PyExc_RuntimeError, "Failed to add edge detection");
      return NULL;
    }
  }

  if (cb_func != NULL)
    if (add_py_callback(gpio, cb_func) != 0)
      return NULL;

  Py_RETURN_NONE;
}

// python function remove_event_detect(gpio)
static PyObject *py_remove_event_detect(PyObject *self, PyObject *args)
{
  unsigned int gpio;
  int channel;
  struct py_callback *cb = py_callbacks;
  struct py_callback *temp;
  struct py_callback *prev = NULL;

  if (!PyArg_ParseTuple(args, "i", &channel))
    return NULL;

	if (get_gpio_number(channel, &gpio))
    return NULL;

  // remove all python callbacks for gpio
  while (cb != NULL)
  {
    if (cb->gpio == gpio)
    {
      Py_XDECREF(cb->py_cb);
      if (prev == NULL)
        py_callbacks = cb->next;
      else
        prev->next = cb->next;

      temp = cb;
      cb = cb->next;
      free(temp);
    } else {
      prev = cb;
      cb = cb->next;
    }
  }

  if (check_gpio_priv())
    return NULL;

  remove_edge_detect(gpio);
  Py_RETURN_NONE;
}

// python function value = event_detected(channel)
static PyObject *py_event_detected(PyObject *self, PyObject *args)
{
  unsigned int gpio;
  int channel;

  if (!PyArg_ParseTuple(args, "i", &channel))
    return NULL;

	if (get_gpio_number(channel, &gpio))
    return NULL;

  if (event_detected(gpio))
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

// python function py_wait_for_edge(gpio, edge)
static PyObject *py_wait_for_edge(PyObject *self, PyObject *args)
{
  unsigned int gpio;
  int channel, edge, result;
  char error[30];

  if (!PyArg_ParseTuple(args, "ii", &channel, &edge))
    return NULL;

	if (get_gpio_number(channel, &gpio))
    return NULL;

  // check channel is setup as an input
  if (gpio_direction[gpio] != INPUT)
  {
    PyErr_SetString(PyExc_RuntimeError, "You must setup() the GPIO channel as an input first");
    return NULL;
  }
  // is edge a valid value?
  edge -= PY_EVENT_CONST_OFFSET;
  if (edge != RISING_EDGE && edge != FALLING_EDGE && edge != BOTH_EDGE)
  {
    PyErr_SetString(PyExc_ValueError, "The edge must be set to RISING, FALLING or BOTH");
    return NULL;
  }

  if (check_gpio_priv())
    return NULL;

  Py_BEGIN_ALLOW_THREADS // disable GIL
  result = blocking_wait_for_edge(gpio, edge);
  Py_END_ALLOW_THREADS   // enable GIL

  if (result == 0) {
    Py_INCREF(Py_None);
    return Py_None;
  } else if (result == 1) {
    PyErr_SetString(PyExc_RuntimeError, "Edge detection events already enabled for this GPIO channel");
    return NULL;
  } else {
    sprintf(error, "Error #%d waiting for edge", result);
    PyErr_SetString(PyExc_RuntimeError, error);
    return NULL;
  }
  Py_RETURN_NONE;
}

static const char moduledocstring[] = "GPIO functionality for OrangePi boards using Python";

PyMethodDef opi_gpio_methods[] = {
  {"setboard", py_setopi, METH_VARARGS, "Set up OrangePi board model to use."},
  {"getboard", py_getopi, METH_VARARGS, "Get OrangePi board model in use."},
  {"setmode", py_setmode, METH_VARARGS, "Set up numbering mode to use for channels.\nBOARD - Use OrangePi board numbers\nBCM   - Use Broadcom GPIO 00..nn numbers\nSOC   - Use SUNXI port numbers"},
  {"getmode", py_getmode, METH_VARARGS, "Get numbering mode used for channel numbers.\nReturns BOARD, BCM, SOC or None"},
  {"setwarnings", py_setwarnings, METH_VARARGS, "Enable or disable warning messages"},
  {"setup", (PyCFunction)py_setup_channel, METH_VARARGS | METH_KEYWORDS, "Set up the GPIO channel, direction and (optional) pull/up down control\nchannel        - either board pin number or BCM number depending on which mode is set.\ndirection      - INPUT or OUTPUT\n[pull_up_down] - PUD_OFF (default), PUD_UP or PUD_DOWN\n[initial]      - Initial value for an output channel"},
  {"cleanup", (PyCFunction)py_cleanup, METH_VARARGS | METH_KEYWORDS, "Clean up by resetting all GPIO channels that have been used by this program to INPUT with no pullup/pulldown and no event detection\n[channel] - individual channel to clean up.  Default - clean every channel that has been used."},
  {"output", py_output_gpio, METH_VARARGS, "Output to a GPIO channel\nchannel - either board pin number or BCM number depending on which mode is set.\nvalue   - 0/1 or False/True or LOW/HIGH"},
  {"input", py_input_gpio, METH_VARARGS, "Input from a GPIO channel.  Returns HIGH=1=True or LOW=0=False\nchannel - either board pin number or BCM number depending on which mode is set."},
  {"gpio_function", py_gpio_function, METH_VARARGS, "Return the current GPIO function (IN, OUT, Multiplexing function ID)\nchannel - either board pin number or GPIO number depending on which mode is set."},
  {"gpio_function_name", py_gpio_function_string, METH_VARARGS, "Return the current GPIO function (IN, OUT, Multiplexing function) as string\nchannel - either board pin number or GPIO number depending on which mode is set."},
  {"add_event_detect", (PyCFunction)py_add_event_detect, METH_VARARGS | METH_KEYWORDS, "Enable edge detection events for a particular GPIO channel.\nchannel      - either board pin number or BCM number depending on which mode is set.\nedge         - RISING, FALLING or BOTH\n[callback]   - A callback function for the event (optional)\n[bouncetime] - Switch bounce timeout in ms for callback"},
  {"remove_event_detect", py_remove_event_detect, METH_VARARGS, "Remove edge detection for a particular GPIO channel\nchannel - either board pin number or BCM number depending on which mode is set."},
  {"event_detected", py_event_detected, METH_VARARGS, "Returns True if an edge has occured on a given GPIO.  You need to enable edge detection using add_event_detect() first.\nchannel - either board pin number or BCM number depending on which mode is set."},
  {"add_event_callback", (PyCFunction)py_add_event_callback, METH_VARARGS | METH_KEYWORDS, "Add a callback for an event already defined using add_event_detect()\nchannel      - either board pin number or BCM number depending on which mode is set.\ncallback     - a callback function"},
  {"wait_for_edge", py_wait_for_edge, METH_VARARGS, "Wait for an edge.\nchannel - either board pin number or BCM number depending on which mode is set.\nedge    - RISING, FALLING or BOTH"},
  {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef opigpiomodule = {
   PyModuleDef_HEAD_INIT,
   "OPi.GPIO",       // name of module
   moduledocstring,  // module documentation, may be NULL
   -1,               // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
   opi_gpio_methods
};
#endif

#if PY_MAJOR_VERSION > 2
PyMODINIT_FUNC PyInit_GPIO(void)
#else
PyMODINIT_FUNC initGPIO(void)
#endif
{
   int i;
   PyObject *module = NULL;

#if PY_MAJOR_VERSION > 2
   if ((module = PyModule_Create(&opigpiomodule)) == NULL)
      return NULL;
#else
   if ((module = Py_InitModule3("OPi.GPIO", opi_gpio_methods, moduledocstring)) == NULL)
      return;
#endif

  define_constants(module);

  for (i=0; i<383; i++)
    gpio_direction[i] = -1;

  // Add PWM class
  if (PWM_init_PWMType() == NULL)
#if PY_MAJOR_VERSION > 2
    return NULL;
#else
    return;
#endif

  Py_INCREF(&PWMType);
  PyModule_AddObject(module, "PWM", (PyObject*)&PWMType);

  if (!PyEval_ThreadsInitialized())
    PyEval_InitThreads();

  // register exit functions - last declared is called first
  if (Py_AtExit(cleanup) != 0)
  {
    setup_error = 1;
    cleanup();
#if PY_MAJOR_VERSION > 2
    return NULL;
#else
    return;
#endif
  }

  if (Py_AtExit(event_cleanup_all) != 0)
  {
    setup_error = 1;
    cleanup();
#if PY_MAJOR_VERSION > 2
    return NULL;
#else
    return;
#endif
  }

#if PY_MAJOR_VERSION > 2
 return module;
#else
 return;
#endif
}
