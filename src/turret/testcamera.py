#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from camera import CameraClass


class unit_tests_camera(unittest.TestCase):

    def __init__(self, *args):
        super(unit_tests_camera, self).__init__(*args)
        self.camera = CameraClass()

    def test_camera(self):
        '''
            Test if camera can see a target
        '''
        self.assertTrue(self.camera.visible_target())

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
