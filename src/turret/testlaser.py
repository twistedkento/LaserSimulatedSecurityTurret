#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from laser import LaserClass


class unit_tests_laser(unittest.TestCase):

    def __init__(self, *args):
        super(unit_tests_laser, self).__init__(*args)
        self.laser = LaserClass()

    def test_laser(self):
        '''
            Test if laser can fire
        '''
        self.laser.fire()
        self.assertTrue(self.laser.get_powerstate())
        time.sleep(3)
        self.laser.cleanup()

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)

