#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
        self.servo = ServoClass()
        self.camera = CameraClass()
        self.manualmode = False

    def turn_left(self):
        print(self.servo)
        self.servo.turn_left()

    def turn_right(self):
        print(self.servo)
        self.servo.turn_right()

    def get_angle(self):
        return self.servo.angle

    def fire_laser(self):
        self.laser.fire()

    def restart(self):
        print("Restarting software...")

    def toggle_mode(self):
        self.manualmode = not self.manualmode
        Print("Manualmode: " + str(self.manualmode))

    def run(self):
        if self.camera.visible_target():
            self.fire_laser()
        self.turn_right()
        self.turn_left()

    def start(self, time):
        for _ in range(time * 100, 0, -1):
            self.run()
