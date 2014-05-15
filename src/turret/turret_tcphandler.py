import socketserver
from turret_command import TurretCommand as command

class TurretTCPHandler(socketserver.BaseRequestHandler):

    def setup(self):
        self.server.connected_clients += 1

    def handle(self):
        while True:
            try:
                data = self.request.recv(1024)
                if not data:
                    break
                self.server.command_queue.put(command(data))
            except:
                print("ERRORRAZ")
                break

            #Sending ok signal!
            #This avoids getting data accumulation on network lag.
            self.request.send((1).to_bytes(1, 'little'))

    def finish(self):
        self.server.connected_clients -= 1
