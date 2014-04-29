import socket

def connect():
    HOST, PORT = "129.16.201.119", 9999
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((HOST, PORT))        
    except ConnectionRefusedError:
        print("Error connecting to server")
    return sock