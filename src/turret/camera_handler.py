#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
from subprocess import Popen,PIPE


class CameraHandler(object):
    '''
        Class for the cameralogic servervise
    '''
    def __init__(self):
        super(CameraHandler, self).__init__()
        self.__video_command = None
        self.__send_command = None
        self.__connected_client = None

    def send_camera(self, caddr):
        if self.__connected_client == caddr[0]:
            self.__video_command.terminate()
            self.__send_command.terminate()
            self.__connected_client = None
        else:
            self.__video_command = Popen(['/opt/vc/bin/raspivid', '-n','-w','800','-h', '600','-fps','24','-t','0','-o','-'], stdout=PIPE, close_fds=False)
            self.__send_command = Popen(['socat', '-', 'udp-sendto:' + caddr[0] + ':5001' ], stdin=self.__video_command.stdout,close_fds=False)
            self.__connected_client = caddr[0]
