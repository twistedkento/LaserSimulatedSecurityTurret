#!/usr/bin/env python
# -*- coding: utf-8 -*-

from picamera import PiCamera
import time


class CameraClass(object):
    '''
        docstring for CameraClass
    '''
    def __init__(self):
        super(CameraClass, self).__init__()

    def visible_target(self):
        '''
            Returns true if target is visible
        '''
        return True

if __name__ == '__main__':
    camera = PiCamera()
    try:
        camera.start_preview()
        time.sleep(10)
        camera.stop_preview()
    finally:
        camera.close()
