#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
from servo import ServoClass
from laser import LaserClass
from camera import CameraClass

class TurretClass(object):
    '''
        docstring for TurretClass
    '''
    def __init__(self):
        super(TurretClass, self).__init__()
        self.laser = LaserClass()
        self.servo = ServoClass(0)
        self.camera = CameraClass()
        self.manualmodeGet re = False

    def turn_left(self):
        '''
            Turns the turret left
        '''
        print(self.servo)
        self.servo.turn_left()

    def turn_right(self):
        '''
            Turns the turret right
        '''
        print(self.servo)
        self.servo.turn_right()

    def get_angle(self):
        '''
            TODO: docstring
        '''
        return self.servo.angle

    def fire_laser(self):
        '''
            TODO: docstring
        '''
        self.laser.fire()

    def restart(self):
        '''
            TODO: docstring
        '''
        print("Restarting software...")

    def toggle_mode(self):
        '''
            TODO: docstring
        '''
        self.manualmode = not self.manualmode
        print("Manualmode: " + str(self.manualmode))

    def run(self):
        '''
            Continously updates the turret depending on mode
        '''
        if self.manualmode:
            self.manual()
        else:
            self.auto_aim()
    
    def manual(self):
        '''
            Takes input for steering and firing
        '''
        #   TODO: REPLACE, currently just exampe movement
        for i in range(150):
            self.turn_right()
            if i % 5 == 0:
                time.sleep(0.02)
        for i in range(150):
            self.turn_left()
            if i % 5 == 0:
                time.sleep(0.02)
    
    def auto_aim(self):
        '''
            Automaticly aims at target and shoots
        '''
        if self.camera.visible_target():
            self.fire_laser()
        

    def start_autoaim(self, time_s):
        '''
            Runs the autoaim mode in "time_s" seconds
        '''
        self.manualmode = False
        start = time.process_time()
        while (time.process_time() - start) < time_s :
            self.run()
