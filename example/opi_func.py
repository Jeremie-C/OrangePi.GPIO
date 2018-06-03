#!/usr/bin/env python
# -*- coding: utf-8 -*-

import OPi.GPIO as GPIO
from time import sleep

GPIO.setboard(GPIO.ZERO)
GPIO.setmode(GPIO.BOARD)

print(GPIO.gpio_function(3))
sleep(0.1)
GPIO.setup(3, GPIO.IN)
sleep(0.1)
print(GPIO.gpio_function(3))
sleep(0.1)
GPIO.cleanup()

