/*
Copyright (c) 2013-2014 Ben Croston

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
#include "boards.h"
#include "common.h"

int board_type = BOARD_UNKNOWN;
int gpio_mode = MODE_UNKNOWN;
int setup_error = 0;
int module_setup = 0;

/* Physical pin to BCM channel */
const int phys_To_BCM[41] = {
  -1, -1, -1, 2, -1, 3, -1, 4, 14, -1, 15, 17, 18, 27, -1, 22, 23, -1, 24, 10,
	-1, 9, 25, 11, 8, -1, 7, 0, 1, 5, -1, 6, 12, 13, -1, 19, 16, 26, 20, -1, 21
};

int is_valid_raw_port(int channel)
{
	if (channel >= 0 && channel < 23) return 1; // PA
	if (channel >= 64 && channel < 84) return 3; // PC
	if (channel >= 96 && channel < 124) return 4; // PD
	if (channel >= 128 && channel < 147) return 5; // PE
	if (channel >= 160 && channel < 167) return 6; // PF
	if (channel >= 192 && channel < 207) return 7; // PG
	if (channel >= 352 && channel < 365) return 12; // PL
	return 0;
}

int get_gpio_number(int channel, unsigned int *gpio)
{
  // check channel number is in range
	if ( (gpio_mode == BCM && (channel < 0 || channel > 27))
	|| (gpio_mode == BOARD && (channel < 1 || channel > 40)) )
	{
		PyErr_SetString(PyExc_ValueError, "The channel sent is invalid");
		return 4;
	}
  // Mode BCM
  if (gpio_mode == BCM)
  {
    unsigned int i;
    for (i=0; i < 41; i++)
    {
      if (*(phys_To_BCM + i) == channel) {
        channel = i;  // Set channel to BOARD PIN
        break;
      }
    }
  }
  // Mode BOARD
  if (gpio_mode == BOARD || gpio_mode == BCM )
	{
    if (*(*pin_to_gpio + channel) == -1)
		{
			PyErr_SetString(PyExc_ValueError, "The channel sent is invalid");
			return 5;
		}
		else
		{
			*gpio = *(*pin_to_gpio + channel);	//pin_to_gpio is initialized in py_gpio.c, the last several lines
		}
  }
  // Mode SOC
  else if (gpio_mode == MODE_SOC)
  {
    if (!is_valid_raw_port(channel))
    {
      PyErr_SetString(PyExc_ValueError, "The channel sent does not exist");
      return 5;
    }
    *gpio = channel;
  }
	else
	{
		// setmode() has not been run
		PyErr_SetString(PyExc_RuntimeError, "Please set pin numbering mode using GPIO.setmode(GPIO.BOARD) or GPIO.setmode(GPIO.BCM) or GPIO.setmode(GPIO.SOC)");
		return 3;
	}

  if (OPiGPIODebug)
    printf("GPIO = %d\n", *gpio);

  return 0;
}

int check_gpio_priv(void)
{
	// check module has been imported cleanly
	if (setup_error)
	{
		PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
		return 1;
	}
	// check mmap setup has worked
	if (!module_setup)
	{
		PyErr_SetString(PyExc_RuntimeError, "No access to /dev/mem.  Try running as root!");
		return 2;
	}
	return 0;
}
