#!/usr/bin/env python
# -*- coding: utf-8 -*-
try:
    import RPi.GPIO as GPIO
except:
    pass
import datetime
import threading
from sound import SoundClass

class LaserClass(object):
    '''
        Class that handles the hardware laser module
    '''

    def __init__(self):
        super(LaserClass, self).__init__()
        self.__powerstate = False
        self.pin = 26
        try:
            GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.pin, GPIO.OUT)
        except:
            pass

    def turn_on(self):
        '''
            Turns laser on
        '''
        self.__powerstate = True
        try:
            GPIO.output(self.pin, True)
        except:
            pass

    def turn_off(self):
        '''
            Turns laser off
        '''
        self.__powerstate = False
        try:
            GPIO.output(self.pin, False)
        except:
            pass

    def get_powerstate(self):
        '''
            returns on/off state of laser
        '''
        return self.__powerstate

    def fire(self):
        '''
            Turns on laser if off
        '''
        if not self.__powerstate:
            self.turn_on()
            t = threading.Timer(0.5, self.turn_off)
            t.start()
            return True
        else:
            return False
        
#    def update(self):
#        print("update")
#        if self.__powerstate:
#            delta = datetime.datetime.now() - self.lastfire
#            if delta.microseconds >= 500000:
#                self.turn_off()

    def cleanup(self):
        try:
            GPIO.cleanup(self.pin)
        except:
            pass

