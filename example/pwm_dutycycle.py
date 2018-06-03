#!/usr/bin/env python
# -*- coding: utf-8 -*-

import OPi.GPIO as GPIO
from time import sleep

GPIO.setboard(GPIO.ZERO)
GPIO.setmode(GPIO.BOARD)    # set up BOARD BCM numbering
GPIO.setup(26, GPIO.OUT)    # set pin 26 as an output (LED)

p = GPIO.PWM(26, 10)        # new PWM on channel=26 frequency=10Hz
p.start(0)
try:
    while 1:
        for dc in range(0, 101, 5):
            p.ChangeDutyCycle(dc)
            sleep(0.1)
        for dc in range(100, -1, -5):
            p.ChangeDutyCycle(dc)
            sleep(0.1)
except KeyboardInterrupt:
    pass
p.stop()
GPIO.output(26, 0)
GPIO.cleanup()
