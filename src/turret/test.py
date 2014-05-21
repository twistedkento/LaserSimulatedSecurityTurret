#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from camera import CameraClass
#from turret import TurretClass
from servo import ServoClass
from laser import LaserClass


class unit_tests(unittest.TestCase):

    def __init__(self, *args):
        super(unit_tests, self).__init__(*args)
        self.camera = CameraClass()
        self.laser = LaserClass()
        #self.turret = TurretClass()
        self.servo = ServoClass(0)
        self.servob = ServoClass(1)
    
    def test_camera(self):
        '''
            Test if camera can see a target
        '''
        self.assertTrue(self.camera.visible_target())
    
    #def test_servo(self):
        '''
            #Tries servo by rotating randomly
        '''
        #for i in range(60):
            #self.servo.go_random()
            #self.servob.go_random()
            #self.servo.go_circle(i*6, 0)
            #self.servob.go_circle(i*6, 1)
            #time.sleep(0.2)
        #self.assertTrue(self.servo.angle <= self.servo._ServoClass__max_value)
        #self.assertTrue(self.servo.angle >= self.servo._ServoClass__min_value)
        #self.servo.reset()
        #self.servob.reset()

    def test_fire_laser(self):
        '''
            Tests the laser by firing it once
        '''
        self.laser.fire()
        for j in range(25):
            for i in range(20):
                self.servob.increase()
            time.sleep(0.05)
            for i in range(20):
                self.servob.decrease()
            time.sleep(0.1)
        self.laser.cleanup()

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
