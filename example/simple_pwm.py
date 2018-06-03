#!/usr/bin/env python
# -*- coding: utf-8 -*-

import OPi.GPIO as GPIO
from time import sleep

GPIO.setboard(GPIO.ZERO)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(26, GPIO.OUT)

p = GPIO.PWM(26, 0.5)               # channel=26 frequency=0.5Hz
p.start(1)
raw_input('Press return to stop:')  # use input for Python 3
p.stop()
GPIO.cleanup()
