#!/usr/bin/env python
# -*- coding: utf-8 -*-

class ServoClass(object):
    '''
        docstring for ServoClass
    '''
    def __init__(self, pin=None):
        self.angle = 90
        self.pin = pin

    def __str__(self):
        return "Servo nr: {0}, Angle: {1}".format(self.pin, self.angle)

    def turn_left(self):
        '''
            TODO: docstring
        '''
        self.angle = max(self.angle - 1, 0)

    def turn_right(self):
        '''
            TODO: docstring
        '''
        self.angle = min(self.angle + 1, 180)
