#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from turret import TurretClass
from servo import ServoClass
from laser import LaserClass
from camera import CameraClass


class IsOddTests(unittest.TestCase):

    def __init__(self, *args):
        super(IsOddTests, self).__init__(*args)
        self.camera = CameraClass()
        self.laser = LaserClass()
        self.turret = TurretClass()
        self.servo = ServoClass(0)

    def test_camera(self):
        '''
            TODO: docstring
        '''
        self.assertTrue(self.camera.visible_target())

    def test_servo(self):
        '''
            TODO: docstring
        '''
        angle = self.servo.angle
        for a in range(100):
            self.servo.go_random()
            time.sleep(0.20)
        #self.assertTrue(self.servo.angle > angle)
        self.assertTrue(self.servo.angle <= self.servo._ServoClass__max_value)
        self.assertTrue(self.servo.angle >= self.servo._ServoClass__min_value)

    def test_laser(self):
        '''
            TODO: docstring
        '''
        self.laser.turn_on()
        self.assertTrue(self.laser.get_powerstate())
        self.laser.turn_off()
        self.assertFalse(self.laser.get_powerstate())

    def test_fire_laser(self):
        '''
            TODO: docstring
        '''
        self.turret.fire_laser()

    def test_automatic_firing(self):
        '''
            TODO: docstring
        '''
        self.turret.start(1)

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
