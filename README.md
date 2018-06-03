# OrangePi.GPIO

This is a modified version of **RPi.GPIO** for Orange Pi Boards.

It is based on the original [RPi.GPIO](https://pypi.python.org/pypi/RPi.GPIO).

## Installation

#### With PIP

    sudo pip install OrangePi.GPIO

#### Manual

    sudo apt-get update
    sudo apt-get install python-dev git
    git clone https://github.com/Jeremie-C/OrangePi.GPIO
    cd /OrangePi.GPIO
    sudo python setup.py install

## Supported Boards

* OPi ZERO
* OPi ZERO PLUS
* OPi ZERO PLUS2 H3
* OPi ZERO PLUS2 H5
* OPi R1
* OPi PC & PC PLUS
* OPi ONE
* OPi LITE
* OPi PC2
* OPi PRIME

## Usage

Same as RPi.GPIO but with a new function to choose OrangePi Board.

    import OPi.GPIO as GPIO
    GPIO.setboard(GPIO.ZEROPLUS)
    GPIO.setmode(GPIO.BOARD)
    GPIO.output(5, 1)

Many demo is on the example folder
