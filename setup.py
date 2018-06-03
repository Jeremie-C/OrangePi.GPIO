"""
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
"""

from distutils.core import setup, Extension

classifiers = ['Development Status :: 3 - Alpha',
               'Operating System :: POSIX :: Linux',
               'License :: OSI Approved :: MIT License',
               'Intended Audience :: Developers',
               'Programming Language :: Python :: 2.6',
               'Programming Language :: Python :: 2.7',
               'Programming Language :: Python :: 3',
               'Topic :: Software Development',
               'Topic :: Home Automation',
               'Topic :: System :: Hardware']

with open("README.md", "r") as fh:
      long_description = fh.read()

setup(name             = 'OrangePi.GPIO',
      version          = '0.6.3',
      author           = 'Jeremie-C',
      description      = 'A module to control OrangePi GPIO channels',
      long_description=long_description,
      long_description_content_type="text/markdown",
      license          = 'MIT',
      keywords         = 'OrangePi GPIO',
      url              = 'https://github.com/Jeremie-C/OrangePi.GPIO',
      classifiers      = classifiers,
      packages         = ['OPi'],
      ext_modules = [Extension('OPi.GPIO', ['source/py_gpio.c', 'source/c_gpio.c', 'source/boards.c', 'source/common.c', 'source/constants.c', 'source/soft_pwm.c', 'source/py_pwm.c', 'source/event_gpio.c'])])
