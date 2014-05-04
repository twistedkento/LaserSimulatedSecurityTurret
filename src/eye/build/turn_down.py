import os
import sys
if os.getcwd() not in sys.path:
    sys.path.append(os.getcwd())
print("down")
import socket
import time
from turret_command import TurretCommand as command

servo = command.ServoState
#if __name__ == '__main__':
#Is host and socket required even if connection is in another file?
#HOST, PORT = "129.16.201.119", 9999
#sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Increase Servo x (left)
data = command()
data.servo_y = servo.dec
#Send keys
conn.send(bytes(data))
status = conn.recv(1)
