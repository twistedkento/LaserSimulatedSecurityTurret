#!/usr/bin/env python
# -*- coding: utf-8 -*-

class LaserClass(object):
    '''
        docstring for LaserClass
    '''

    def __init__(self):
        super(LaserClass, self).__init__()
        self.__powerstate = False

    def turn_on(self):
        '''
            TODO: docstring
        '''
        self.__powerstate = True
        #print("Powerstate: " + str(self.__powerstate))

    def turn_off(self):
        '''
            TODO: docstring
        '''
        self.__powerstate = False
        #print("Powerstate: " + str(self.__powerstate))

    def get_powerstate(self):
        '''
            TODO: docstring
        '''
        return self.__powerstate

    def fire(self):
        '''
            TODO: docstring
        '''
        self.turn_on()
        #print("IMMA FIRIN' MA LAZOR!!!")
        self.turn_off()
