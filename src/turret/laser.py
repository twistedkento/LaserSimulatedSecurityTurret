#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sound import Sound
try:
    import RPi.GPIO as GPIO
except:
    pass
import threading

class LaserClass(object):
    '''
        Class that handles the hardware laser module
    '''

    def __init__(self):
        super(LaserClass, self).__init__()
        self.__powerstate = False
        self.__sound = Sound()
        self.__pin=26
        try:
            GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.__pin, GPIO.OUT)
        except:
            pass

    def turn_on(self):
        '''
            Turns laser on
        '''
        self.__powerstate = True
        try:
            GPIO.output(self.__pin, True)
        except:
            pass

    def turn_off(self):
        '''
            Turns laser off
        '''
        self.__powerstate = False
        try:
            GPIO.output(self.__pin, False)
        except:
            pass

    def get_powerstate(self):
        '''
            returns on/off state of laser
        '''
        return self.__powerstate

    def fire(self):
        '''
            Turns on and off laser + beeps
        '''
        self.turn_on()
        self.__sound.play_sound("laser")
        t = threading.Timer(2, self.turn_off)
        t.start()
        
    def cleanup(self):
        try:
            GPIO.cleanup(self.__pin)
        except:
            pass

