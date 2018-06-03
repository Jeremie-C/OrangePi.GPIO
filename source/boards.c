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

const char* FUNCTIONS[] = {
  "INPUT","OUTPUT","RESERVED","UNKNOWN","SEE_MANUAL", // 0-4
  "TWI0_SCK","TWI0_SDA",                              // 5-6
  "TWI1_SCK","TWI1_SDA",                              // 7-8
  "TWI2_SCK","TWI2_SDA",                              // 9-10
  "S_TWI_SCK", "S_TWI_SDA",                           // 11-12
  "SPI0_CS","SPI0_CLK","SPI0_MOSI","SPI0_MISO",       // 13-16
  "SPI1_CS","SPI1_CLK","SPI1_MOSI","SPI1_MISO",       // 17-20
  "UART0_TX","UART0_RX",                              // 21-22
  "UART1_TX","UART1_RX","UART1_RTS","UART1_CTS",      // 23-26
  "UART2_TX","UART2_RX","UART2_RTS","UART2_CTS",      // 27-30
  "UART3_TX","UART3_RX","UART3_RTS","UART3_CTS",      // 31-34
  "S_UART_TX","S_UART_RX",                            // 35-36
  "PWM0","PWM1","S_PWM",                              // 37-39
  "IO DISABLED"
};

/* Get Alt Function Name */
int gpio_function_name(int gpio, int func, int board)
{
  int str;

  switch (func)
  {
    case 0 : str = 0;  break;
    case 1 : str = 1; break;
    case 2 :
      switch (gpio)
      {
        case 0 : str = 27; break;
        case 1 : str = 28; break;
        case 2 : str = 29; break;
        case 3 : str = 30; break;
        case 4 : str = 21; break;
        case 5 : str = 22; break;
        case 11 : str = 5; break;
        case 12 : str = 6; break;
        case 13 : str = 17; break;
        case 14 : str = 18; break;
        case 15 : str = 19; break;
        case 16 : str = 20; break;
        case 198 : str = 25; break;
        case 199 : str = 26; break;
        case 200 : str = 27; break;
        case 201 : str = 28; break;
        case 352 : str = 11; break;
        case 353 : str = 12; break;
        case 354 : str = 35; break;
        case 355 : str = 36; break;
        case 362 : str = 39; break;
        default : str = 2; break;
      }
      break;
    case 3 :
      switch (gpio)
      {
        case 5 : str = 37; break;
        case 6 : // PWM1 only H3
          if (board == 1 || board == 3 || board == 4) {
            str = 38;
          } else {
            str = 4;
          }
          break;
        case 13 : str = 31; break;
        case 14 : str = 32; break;
        case 15 : str = 33; break;
        case 16 : str = 34; break;
        case 18 : str = 7; break;
        case 19 : str = 8; break;
        case 64 : str = 15; break;
        case 65 : str = 16; break;
        case 66 : str = 14; break;
        case 67 : str = 13; break;
        case 140 : str = 9; break;
        case 141 : str = 10; break;
        case 162 : str = 21; break;
        case 164 : str = 22; break;
        default : str = 2; break;
      }
      break;
    case 4 :
      switch (gpio)
      {
        case 68 : // H5 Only
          if (board == 2 || board == 5 || board == 6) {
            str = 16;
          } else {
            str = 2;
          }
          break;
        default : str = 2; break;
      }
      break;
    case 5 : str = 2; break;
    case 6 : str = 4; break;
    case 7 : str = 40; break;
    default : str = 3; break;
  }
  return str;
}
