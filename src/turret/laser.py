#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sound

class LaserClass(object):
    '''
        Class that handles the hardware laser module
    '''

    def __init__(self):
        super(LaserClass, self).__init__()
        self.__powerstate = False

    def turn_on(self):
        '''
            Turns laser on
        '''
        self.__powerstate = True
        #   TODO : Turn on laser 

    def turn_off(self):
        '''
            Turns laser off
        '''
        self.__powerstate = False
        #   TODO : Turn off laser

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
        #   TODO: Play speaker sound
        sound.play_sound(0)
        #print("IMMA FIRIN' MA LAZOR!!!")
        self.turn_off()
