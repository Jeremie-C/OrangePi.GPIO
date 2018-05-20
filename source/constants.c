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

	board = Py_BuildValue("i", BOARD);
	PyModule_AddObject(module, "BOARD", board);

	bcm = Py_BuildValue("i", BCM);
	PyModule_AddObject(module, "BCM", bcm);

	soc = Py_BuildValue("i", MODE_SOC);
	PyModule_AddObject(module, "SOC", soc);

	pud_off = Py_BuildValue("i", PUD_OFF + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_OFF", pud_off);

	pud_up = Py_BuildValue("i", PUD_UP + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_UP", pud_up);

	pud_down = Py_BuildValue("i", PUD_DOWN + PY_PUD_CONST_OFFSET);
	PyModule_AddObject(module, "PUD_DOWN", pud_down);

	bunknown = Py_BuildValue("i", BOARD_UNKNOWN);
	PyModule_AddObject(module, "BUNKNOWN", bunknown);

	bzeroh2 = Py_BuildValue("i", ZERO);
	PyModule_AddObject(module, "ZERO", bzeroh2);

	br1 = Py_BuildValue("i", R1);
	PyModule_AddObject(module, "R1", br1);

	bzeroplus = Py_BuildValue("i", ZEROPLUS);
	PyModule_AddObject(module, "ZEROPLUS", bzeroplus);

	bzeroplus5 = Py_BuildValue("i", ZEROPLUS2H5);
	PyModule_AddObject(module, "ZEROPLUS2H5", bzeroplus5);

	bzeroplus3 = Py_BuildValue("i", ZEROPLUS2H3);
	PyModule_AddObject(module, "ZEROPLUS2H3", bzeroplus3);

	bpc = Py_BuildValue("i", PC);
	PyModule_AddObject(module, "PC", bpc);

	bpcplus = Py_BuildValue("i", PCPLUS);
	PyModule_AddObject(module, "PCPLUS", bpcplus);

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
