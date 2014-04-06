#!/usr/bin/env python
# -*- coding: utf-8 -*-

class ServoClass(object):
    '''
        docstring for ServoClass
    '''
    def __init__(self):
        self.angle = 0

    def __str__(self):
        return "Servo nr: {0}, Angle: {1}".format(None, self.angle)

    def turn_left(self):
        self.angle -= 1

    def turn_right(self):
        self.angle += 1
