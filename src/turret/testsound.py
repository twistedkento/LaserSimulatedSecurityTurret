#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import time
from sound import SoundClass

class unit_tests_sound(unittest.TestCase):

    def __init__(self, *args):
        super(unit_tests_sound, self).__init__(*args)
        self.sound = SoundClass()

    def test_sound(self):
        '''
            Tests sound
        '''
        self.sound.play_sound("laser")
        time.sleep(0.5)

if __name__ == '__main__':
    unittest.main(buffer=True, verbosity=2)
