#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import socketserver
from multiprocessing import Process, Queue
from turret_udphandler import TurretUDPHandler
from turret_command import TurretCommand as command
from servo import ServoClass
from laser import LaserClass
from sound import SoundClass
from camera_handler import CameraHandler
import pdb

class TurretClass(object):
    '''
        docstring for TurretClass
    '''
    def __init__(self):
        super(TurretClass, self).__init__()
        self.laser = LaserClass()
        self.servo_x = ServoClass(pin=0)
        self.servo_y = ServoClass(pin=1)
        self.camera_handler = CameraHandler()
        self.sound = SoundClass()
        self.manualmode = False

        servostate = command.ServoState
        laserstate = command.LaserState
        extrastate = command.ExtraState
        self.command_calls = dict()
        self.command_calls['servo_x'] = {
                servostate.inc: self.turn_right,
                servostate.dec: self.turn_left,
                servostate.off: lambda: None,
                servostate.reset: self.horizontal_reset}
        self.command_calls['servo_y'] = {
                servostate.inc: self.turn_up,
                servostate.dec: self.turn_down,
                servostate.off: lambda: None,
                servostate.reset: self.vertical_reset}
        self.command_calls['laser'] = {
                laserstate.on: self.fire_laser,
                laserstate.off: lambda: None}
        self.command_calls['extra'] = {
                extrastate.off: lambda x: None,
                extrastate.camera: self.call_camera}

    def turn_left(self):
        '''
            Turns the turret left
        '''
        self.servo_x.increase()

    def turn_right(self):
        '''
            Turns the turret right
        '''
        self.servo_x.decrease()

    def horizontal_reset(self):
        '''
            Returns turret to initial horizontal state
        '''
        self.servo_x.angle_reset()

    def turn_up(self):
        '''
            Turns the turret up
        '''
        self.servo_y.increase()

    def turn_down(self):
        '''
            Turns the turret down
        '''
        self.servo_y.decrease()

    def vertical_reset(self):
        '''
            Returns turret to initial vertical state
        '''
        self.servo_y.angle_reset()

    def get_angle(self):
        '''
            TODO: Is this function needed?
        '''
        return {'x': self.servo_x.angle, 'y': self.servo_y.angle}

    def fire_laser(self):
        '''
            Fire the laser
        '''
        if self.laser.fire():
            print("firing")
            self.sound.play_sound("laser")

    def restart(self):
        '''
            Resets the turret
        '''
        print("Restarting software...")
        self.vertical_reset()
        self.horizontal_reset()
        self.laser.turn_off()

    def toggle_mode(self):
        '''
            TODO: Decide if this is needed?
        '''
        self.manualmode = not self.manualmode
        print("Manualmode: " + str(self.manualmode))
    def call_camera(self, caddr):
        '''
        '''
        self.camera_handler.send_camera(caddr)
        
    
    def handle_command(self, in_command,caddr=None):
        '''
            Handles the command input sent from the client
        '''
        servostate = command.ServoState
        laserstate = command.LaserState
        extrastate = command.ExtraState

        self.command_calls['servo_x'][servostate(in_command.servo_x)]()
        self.command_calls['servo_y'][servostate(in_command.servo_y)]()
        self.command_calls['laser'][laserstate(in_command.laser)]()
        self.command_calls['extra'][extrastate(in_command.extra)](caddr)

    def run(self):
        '''
            Main loop that handles the turret.
        '''
        HOST, PORT = "0.0.0.0", 9999
        server = socketserver.UDPServer((HOST, PORT), TurretUDPHandler)
        try:
            server.command_queue = Queue()
            server.connected_clients = 0
            p = Process(target=server.serve_forever)
            p.start()
            while True:
                msg,extra_info = server.command_queue.get()
                self.handle_command(msg,caddr=extra_info)
        except KeyboardInterrupt:
            print("Exiting server loop")
            p.join()
        finally:
            self.laser.cleanup()
            server.server_close()


if __name__ == '__main__':
    turret = TurretClass()
    turret.run()
