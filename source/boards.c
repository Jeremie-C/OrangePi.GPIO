/*
Copyright (c) 2018 Jeremie-C

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
#include "common.h"
#include "boards.h"

/* OrangePi Zero / Zero+ / R1 / Zero+2H5 */
const int pin_to_gpio_zero[41] = {
  -1, -1, -1, // 0, 1, 2
  12, -1, // 3, 4
  11, -1, // 5, 6
  6, 198, // 7, 8
  -1, 199, // 9, 10
  1, 7, // 11, 12
  0, -1, // 13, 14
  3, 19, // 15, 16
  -1, 18, // 17, 18
  15, -1, // 19, 20
  16, 2, // 21, 22
  14, 13, // 23, 24
  -1, 10, // 25, 26
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

/* OrangePi Zero Plus 2 H3 */
const int pin_to_gpio_zero2[41] = {
  -1, -1, -1, // 0, 1, 2
  12, -1, // 3, 4
  11, -1, // 5, 6
  6, 0, // 7, 8
  -1, 1, // 9, 10
  352, 107, // 11, 12
  353, -1, // 13, 14
  3, 19, // 15, 16
  -1, 18, // 17, 18
  15, -1, // 19, 20
  16, 2, // 21, 22
  14, 13, // 23, 24
  -1, 110, // 25, 26
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

/* OrangePi PC / PC+ / One / Lite / Plus2E */
const int pin_to_gpio_pc[41] = {
  -1, -1, -1, // 0, 1, 2
  12, -1, // 3, 4
  11, -1, // 5, 6
  6, 13, // 7, 8
  -1, 14, // 9, 10
  1, 142, // 11, 12
  0, -1, // 13, 14
  3, 68, // 15, 16
  -1, 71, // 17, 18
  64, -1, // 19, 20
  65, 2, // 21, 22
  66, 67, // 23, 24
  -1, 21, // 25, 26
  19, 18, // 27, 28
  7, -1, // 29, 30
  8, 200, // 31, 32
  9, -1, // 33, 34
  10, 201, // 35, 36
  20, 198, // 37, 38
  -1, 199 // 39, 40
};

/* OrangePi PC2 */
const int pin_to_gpio_pc2[41] = {
  -1, -1, -1, // 0, 1, 2
  12, -1, // 3, 4
  11, -1, // 5, 6
  6, 69, // 7, 8
  -1, 70, // 9, 10
  1, 110, // 11, 12
  0, -1, // 13, 14
  3, 68, // 15, 16
  -1, 71, // 17, 18
  15, -1, // 19, 20
  16, 2, // 21, 22
  14, 13, // 23, 24
  -1, 21, // 25, 26
  19, 18, // 27, 28
  7, -1, // 29, 30
  8, 200, // 31, 32
  9, -1, // 33, 34
  10, 201, // 35, 36
  107, 198, // 37, 38
  -1, 199 // 39, 40
};

/* OrangePi Prime */
const int pin_to_gpio_prime[41] = {
  -1, -1, -1, // 0, 1, 2
  12, -1, // 3, 4
  11, -1, // 5, 6
  6, 69, // 7, 8
  -1, 70, // 9, 10
  1, 110, // 11, 12
  0, -1, // 13, 14
  3, 68, // 15, 16
  -1, 71, // 17, 18
  15, -1, // 19, 20
  16, 2, // 21, 22
  14, 13, // 23, 24
  -1, 72, // 25, 26
  19, 18, // 27, 28
  7, -1, // 29, 30
  8, 73, // 31, 32
  9, -1, // 33, 34
  10, 74, // 35, 36
  107, 75, // 37, 38
  -1, 76 // 39, 40
};
