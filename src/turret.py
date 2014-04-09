#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from servo import ServoClass
from laser import LaserClass
from camera import CameraClass

class TurretClass(object):
    '''
        docstring for TurretClass
    '''
    def __init__(self):
        super(TurretClass, self).__init__()
        self.laser = LaserClass()
        self.servo = ServoClass(0)
        self.camera = CameraClass()
        self.manualmode = False

    def turn_left(self):
        '''
            TODO: docstring
        '''
        print(self.servo)
        self.servo.turn_left()

    def turn_right(self):
        '''
            TODO: docstring
        '''
        print(self.servo)
        self.servo.turn_right()

    def get_angle(self):
        '''
            TODO: docstring
        '''
        return self.servo.angle

    def fire_laser(self):
        '''
            TODO: docstring
        '''
        self.laser.fire()

    def restart(self):
        '''
            TODO: docstring
        '''
        print("Restarting software...")

    def toggle_mode(self):
        '''
            TODO: docstring
        '''
        self.manualmode = not self.manualmode
        print("Manualmode: " + str(self.manualmode))

    def run(self):
        '''
            TODO: docstring
        '''
        if self.camera.visible_target():
            self.fire_laser()
        for i in range(150):
            self.turn_right()
            if i % 5 == 0:
                time.sleep(0.02)
        for i in range(150):
            self.turn_left()
            if i % 5 == 0:
                time.sleep(0.02)

    def start(self, time):
        '''
            TODO: docstring
        '''
        for _ in range(time * 1, 0, -1):
            self.run()
