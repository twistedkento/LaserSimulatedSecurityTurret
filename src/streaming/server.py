#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

os.system("nc -u -l 5001 | mplayer -fps 24 -cache 1024 -")
