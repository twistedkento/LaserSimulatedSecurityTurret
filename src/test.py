#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from turret import TurretClass
from servo import ServoClass
from laser import LaserClass
from camera import CameraClass


class unit_tests(unittest.TestCase):

    def __init__(self, *args):
        super(unit_tests, self).__init__(*args)
        self.camera = CameraClass()
        self.laser = LaserClass()
        self.turret = TurretClass()
        self.servo = ServoClass(0)

    def test_camera(self):
        '''
            Test if camera can see a target
        '''
        self.assertTrue(self.camera.visible_target())

    def test_servo(self):
        '''
            Tries servo by rotating randomly
        '''
        angle = self.servo.angle
        for a in range(10):
            self.servo.go_random()
            time.sleep(0.20)
        self.assertTrue(self.servo.angle <= self.servo._ServoClass__max_value)
        self.assertTrue(self.servo.angle >= self.servo._ServoClass__min_value)

    def test_laser(self):
        '''
            Tests the laser by turning it on and off
        '''
        self.laser.turn_on()
        self.assertTrue(self.laser.get_powerstate())
        self.laser.turn_off()
        self.assertFalse(self.laser.get_powerstate())

    def test_fire_laser(self):
        '''
            Tests the laser by firing it once
        '''
        self.turret.fire_laser()

    def test_automatic_firing(self):
        '''
            Runs automatic firing in 
        '''
        self.turret.start_autoaim(1)

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
