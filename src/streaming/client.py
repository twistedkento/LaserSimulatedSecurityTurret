#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

'''
    Maybe handle ip and that kind of stuff...
'''
from multiprocessing import Process, Lock

def run(self):
    Process(target=self.process_run, args=('129.16.194.248')).start()

def process_run(self,caddr):
    #os.system("/opt/vc/bin/raspivid -u -n -w 800 -h 600 -fps 24 -t 0 -o - | nc 192.168.0.120 5001")
    #os.system('/opt/vc/bin/raspivid -w 800 -h 600 -fps 24 -t 0 -o - | socat - udp-sendto:' + '129.16.194.248' + ':5001')
    os.system('/opt/vc/bin/raspivid -w 800 -h 600 -fps 24 -t 0 -o - | nc -u ' + '129.16.194.248' + ' 5001')
