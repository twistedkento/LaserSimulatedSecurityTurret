#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import socketserver
from multiprocessing import Process, Queue
from turret_tcphandler import TurretTCPHandler
from turret_command import TurretCommand as command
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
        self.servo_x = ServoClass(0)
        self.servo_y = ServoClass(1)
        self.camera = CameraClass()
        self.manualmode = False

        servostate = command.ServoState
        laserstate = command.LaserState
        self.command_calls = dict()
        self.command_calls['servo_x'] = {
                servostate.inc : self.turn_right,
                servostate.dec: self.turn_left,
                servostate.off: lambda: None,
                servostate.reset: self.horizontal_reset}
        self.command_calls['servo_y'] = {
                servostate.inc : self.turn_up,
                servostate.dec: self.turn_down,
                servostate.off: lambda: None,
                servostate.reset: self.vertical_reset}
        self.command_calls['laser'] = {
                laserstate.on: self.laser.turn_on,
                laserstate.off : self.laser.turn_off}

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

    def vertical_reset(self):
        '''
            Returns turret to initial vertical state
        '''
        self.servo_x.reset()

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

    def horizontal_reset(self):
        '''
            Returns turret to initial horizontal state
        '''
        self.servo_y.reset()

    def get_angle(self):
        '''
            TODO: Is this function needed?
        '''
        return {'x': self.servo_x.angle, 'y': self.servo_y.angle}

    def fire_laser(self):
        '''
            Fire the laser
        '''
        self.laser.fire()

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
    
    def handle_command(self, in_command):
        '''
            Handles the command input sent from the client
        '''
        servostate = command.ServoState
        laserstate = command.LaserState

        self.command_calls['servo_x'][servostate(in_command.servo_x)]()
        self.command_calls['servo_y'][servostate(in_command.servo_y)]()
        self.command_calls['laser'][laserstate(in_command.laser)]()

    def run(self):
        '''
            Main loop that handles the turret.
        '''
        HOST, PORT = "0.0.0.0", 9999
        server = socketserver.TCPServer((HOST, PORT), TurretTCPHandler)
        try:
            server.command_queue = Queue()
            server.connected_clients = 0
            p = Process(target=server.serve_forever)
            p.start()
            while True:
                msg = server.command_queue.get()
                self.handle_command(msg)
        except KeyboardInterrupt:
            print("Exiting server loop")
            p.join()
        finally:
            server.server_close()


if __name__ == '__main__':
    turret = TurretClass()
    turret.run()
