#!/usr/bin/env python
# -*- coding: utf-8 -*-
from sound import Sound
import RPi.GPIO as GPIO
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
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(self.__pin, GPIO.OUT)

    def turn_on(self):
        '''
            Turns laser on
        '''
        self.__powerstate = True
        GPIO.output(self.__pin, True)

    def turn_off(self):
        '''
            Turns laser off
        '''
        self.__powerstate = False
        GPIO.output(self.__pin, False)

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
        GPIO.cleanup(self.__pin)

