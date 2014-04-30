import socket

HOST, PORT = "129.16.202.23", 9999
conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
	conn.connect((HOST, PORT))       
	print("connected!")
except ConnectionRefusedError:
	print("Error connecting to server")