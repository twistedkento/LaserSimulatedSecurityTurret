import socket
import os
import sys
sys.path.append(os.getcwd())

HOST, PORT = "localhost", 9999
conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
	conn.connect((HOST, PORT))       
	print("connected!")
except ConnectionRefusedError:
	print("Error connecting to server")
except Exception as e:
    print(e)
