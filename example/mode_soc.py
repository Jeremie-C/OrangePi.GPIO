#!/usr/bin/env python
# -*- coding: utf-8 -*-

import OPi.GPIO as GPIO
from time import sleep          # this lets us have a time delay

GPIO.setboard(GPIO.ZERO)        # Orange Pi Zero board
GPIO.setmode(GPIO.SOC)          # set up SOC numbering

sled = GPIO.PA+17               # Status led is on PA17
GPIO.setup(sled, GPIO.OUT)      # set PA17 as an output (Status led of board)

try:
  while True:
    GPIO.output(sled, 1)        # set port/pin value to 1/HIGH/True
    sleep(0.1)
    GPIO.output(sled, 0)        # set port/pin value to 0/LOW/False
    sleep(0.1)
    GPIO.output(sled, 1)        # set port/pin value to 1/HIGH/True
    sleep(0.1)
    GPIO.output(sled, 0)        # set port/pin value to 0/LOW/False
    sleep(0.5)

except KeyboardInterrupt:
  GPIO.output(sled, 0)
  GPIO.cleanup()                # clean up after yourself
  print ("Bye.")
