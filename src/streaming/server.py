#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import subprocess

running = os.system("nc -u -l -p 5001 | mplayer -cache 1024 -")
#subprocess.check_call('/opt/vc/bin/raspivid -n -w 800 -h 600 -fps 24 -t 0 -o - | socat - udp-sendto:' + '129.16.194.248' + ':5001')
