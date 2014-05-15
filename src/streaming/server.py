#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

os.system("nc -u -l -p 5001 | mplayer -fps 24 -cache 1024 -")
