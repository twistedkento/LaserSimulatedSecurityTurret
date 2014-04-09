#!/usr/bin/env python
# -*- coding: utf-8 -*-

class ServoClass(object):
    '''
        docstring for ServoClass
    '''
    def save_decorator(self, f):
        def inner_func(*args, **kw):
            try:
                return f(*args, **kw)
            finally:
                self.do_save()
        return inner_func

    def __init__(self, pin=None):
        self.__max_value = 230
        self.__min_value = 60
        self.__start_value = 140
        self.angle = 90
        if pin < 0 and pin > 7:
            raise
        self.pin = pin

    def __str__(self):
        return "Servo nr: {0}, Angle: {1}".format(self.pin, self.angle)

    @save_decorator
    def turn_left(self):
        '''
            TODO: docstring
        '''
        self.angle = max(self.angle - 1, self.__min_value)

    @save_decorator
    def turn_right(self):
        '''
            TODO: docstring
        '''
        self.angle = min(self.angle + 1, self.__max_value)

    def do_save(self):
        '''
            TODO:docstring
        '''
        with open('/dev/servoblaster', 'w+') as sblaster:
            sblaster.write('{0}={1}'.format(self.pin, self.angle))
