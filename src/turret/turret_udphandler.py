import socketserver
from turret_command import TurretCommand as command

class ThreadedTurretUDPHandler(object):
    pass

class TurretUDPHandler(socketserver.BaseRequestHandler):

    def handle(self):
        try:
            data = self.request[0]
            #data = self.request.recv(1024)
            self.server.command_queue.put((command(data),self.client_address))
        except:
            print("Exit or error!")
            pass

        #Sending ok signal!
        #This avoids getting data accumulation on network lag.
        self.request[1].sendto((1).to_bytes(1,'little'), self.client_address)
        #self.request.send((1).to_bytes(1, 'little'))
