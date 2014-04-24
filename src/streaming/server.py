#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os

os.system("nc -l 5001 | mplayer -fps 31 -cache 1024 -")
