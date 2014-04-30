import socket
def disconnect(conn):
    #HOST, PORT = "129.16.201.119", 9999
    #sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    conn.close()