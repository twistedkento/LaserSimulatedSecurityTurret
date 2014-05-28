#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import os
from multiprocessing import Process, Lock


class CameraHandler(object):
    '''
        Class for the cameralogic servervise
    '''
    def __init__(self):
        super(CameraHandler, self).__init__()
        self.__video_command = None
        self.__send_command = None
        self.__connected_client = None
        self.__camera_thread = None

    def send_camera(self, caddr):
        if self.__connected_client == caddr[0]:
            self.__camera_thread.terminate()
            self.__connected_client = None
        else:
            self.__camera_thread = Process(target=self.process_run, args=(caddr[0])).start()
            self.__connected_client = caddr[0]

    def process_run(self,caddr):
        os.system('/opt/vc/bin/raspivid -w 800 -h 600 -fps 24 -t 0 -o - | nc -u ' + caddr + ' 5001')
