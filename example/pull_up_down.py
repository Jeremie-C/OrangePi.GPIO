#!/usr/bin/env python
# -*- coding: utf-8 -*-

import OPi.GPIO as GPIO
from time import sleep          # this lets us have a time delay

GPIO.setboard(GPIO.ZERO)        # Orange Pi Zero board
GPIO.setmode(GPIO.BOARD)        # set up BOARD GPIO numbering
GPIO.setup(15, GPIO.IN, pull_up_down=GPIO.PUD_OFF)    # set pin 15 as input (button)
GPIO.setup(11, GPIO.OUT)        # set pin 11 as an output (LED)

try:
    while True:                 # this will carry on until you hit CTRL+C
        if GPIO.input(15):      # if pin 15 == 1
            print "Port 15 is 1/HIGH/True - LED ON"
            GPIO.output(11, 1)  # set port/pin value to 1/HIGH/True
        else:
            print "Port 15 is 0/LOW/False - LED OFF"
            GPIO.output(11, 0)  # set port/pin value to 0/LOW/False
        sleep(0.1)              # wait 0.1 seconds

finally:                        # this block will run no matter how the try block exits
    print("Finally")
    GPIO.output(11, 0)
    GPIO.cleanup()              # clean up after yourself
