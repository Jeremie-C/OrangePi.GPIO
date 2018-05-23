/*
Copyright (c) 2013-2016 Ben Croston

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
#include "constants.h"
#include "common.h"
#include "c_gpio.h"
#include "event_gpio.h"

void define_constants(PyObject *module)
{
	high = Py_BuildValue("i", HIGH);
	PyModule_AddObject(module, "HIGH", high);

	low = Py_BuildValue("i", LOW);
	PyModule_AddObject(module, "LOW", low);

	output = Py_BuildValue("i", OUTPUT);
	PyModule_AddObject(module, "OUT", output);

	input = Py_BuildValue("i", INPUT);
	PyModule_AddObject(module, "IN", input);

	pud_off = Py_BuildValue("i", PUD_OFF + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_OFF", pud_off);

	pud_up = Py_BuildValue("i", PUD_UP + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_UP", pud_up);

	pud_down = Py_BuildValue("i", PUD_DOWN + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_DOWN", pud_down);

	rising_edge = Py_BuildValue("i", RISING_EDGE + PY_EVENT_CONST_OFFSET);
	PyModule_AddObject(module, "RISING", rising_edge);

	falling_edge = Py_BuildValue("i", FALLING_EDGE + PY_EVENT_CONST_OFFSET);
	PyModule_AddObject(module, "FALLING", falling_edge);

	both_edge = Py_BuildValue("i", BOTH_EDGE + PY_EVENT_CONST_OFFSET);
	PyModule_AddObject(module, "BOTH", both_edge);

	board = Py_BuildValue("i", BOARD);
	PyModule_AddObject(module, "BOARD", board);

	bcm = Py_BuildValue("i", BCM);
	PyModule_AddObject(module, "BCM", bcm);

	soc = Py_BuildValue("i", MODE_SOC);
	PyModule_AddObject(module, "SOC", soc);

	bunknown = Py_BuildValue("i", BOARD_UNKNOWN);
	PyModule_AddObject(module, "BUNKNOWN", bunknown);

	bzeroh2 = Py_BuildValue("i", ZERO);
	PyModule_AddObject(module, "ZERO", bzeroh2);
	PyModule_AddObject(module, "R1", bzeroh2);
	PyModule_AddObject(module, "ZEROPLUS", bzeroh2);
	PyModule_AddObject(module, "ZEROPLUS2H5", bzeroh2);

	bzeroplus3 = Py_BuildValue("i", ZEROPLUS2H3);
	PyModule_AddObject(module, "ZEROPLUS2H3", bzeroplus3);

	bpc = Py_BuildValue("i", PC);
	PyModule_AddObject(module, "PC", bpc);
	PyModule_AddObject(module, "PCPLUS", bpc);
	PyModule_AddObject(module, "PLUS2E", bpc);

	bpc2 = Py_BuildValue("i", PC2);
	PyModule_AddObject(module, "PC2", bpc2);

	bprime = Py_BuildValue("i", PRIME);
	PyModule_AddObject(module, "PRIME", bprime);

	version = Py_BuildValue("s", "0.6.3");
	PyModule_AddObject(module, "VERSION", version);

	PyModule_AddObject(module, "PA", Py_BuildValue("i", 0));
	PyModule_AddObject(module, "PC", Py_BuildValue("i", 64));
	PyModule_AddObject(module, "PD", Py_BuildValue("i", 96));
	PyModule_AddObject(module, "PE", Py_BuildValue("i", 128));
	PyModule_AddObject(module, "PF", Py_BuildValue("i", 160));
	PyModule_AddObject(module, "PG", Py_BuildValue("i", 192));
	PyModule_AddObject(module, "PL", Py_BuildValue("i", 352));
}
