#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

'''
    Maybe handle ip and that kind of stuff...
'''
os.system("/opt/vc/bin/raspivid -u -n -w 800 -h 600 -fps 24 -t 0 -o - | nc 192.168.0.120 5001")
