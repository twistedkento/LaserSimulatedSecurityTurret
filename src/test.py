#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
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
        self.servo = ServoClass()

    def test_camera(self):
        self.assertTrue(self.camera.visible_target())

    def test_servo(self):
        angle = self.servo.angle
        for _ in range(1000):
            self.servo.turn_right()
        self.assertTrue(self.servo.angle > angle)
        self.assertTrue(self.servo.angle <= 180)
        self.assertTrue(self.servo.angle >= 0)

    def test_laser(self):
        self.laser.turn_on()
        self.assertTrue(self.laser.get_powerstate())
        self.laser.turn_off()
        self.assertFalse(self.laser.get_powerstate())

    def test_fire_laser(self):
        self.turret.fire_laser()

    def test_automatic_firing(self):
        self.turret.start(50)

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
