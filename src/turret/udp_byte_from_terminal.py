import socket
import sys

HOST, PORT = "129.16.199.161", 9999
data = " ".join(sys.argv[1:])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    sock.sendto((int(data).to_bytes(1,'little')), (HOST, PORT))
    received = sock.recv(1024)

    print("Sent:     {}".format(data))
    print("Received: {}".format(received))
except:
    pass
