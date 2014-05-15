import socket
import time
import curses
from turret_command import TurretCommand as command

servo = command.ServoState
laser = command.LaserState

if __name__ == '__main__':
    #Init curses
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    stdscr.keypad(1)
    stdscr.leaveok(1)

    laserstate = True

    #HOST, PORT = "129.16.194.203", 9999
    HOST, PORT = "127.1", 9999
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect((HOST, PORT))
        stdscr.nodelay(1)
        while True:
            #Handle key input
            data = command()
            if laserstate:
                data.laser = laser.on
            else:
                data.laser = laser.off
            c = stdscr.getch()
            if not c == -1:
                if c == 27:
                    break
                if c == curses.KEY_LEFT:
                    data.servo_x = servo.inc
                    curses.flushinp()
                elif c == curses.KEY_RIGHT:
                    data.servo_x = servo.dec
                    curses.flushinp()
                if c == curses.KEY_UP:
                    data.servo_y = servo.inc
                elif c == curses.KEY_DOWN:
                    data.servo_y = servo.dec
                    curses.flushinp()
                if c == ord(' '):
                    laserstate = not laserstate
                    if laserstate:
                        data.laser = laser.on
                    else:
                        data.laser = laser.off
                    curses.flushinp()
            else:
                curses.flushinp()
                time.sleep(0.1)
                continue
            #Send keys
            stdscr.addstr(0,0,'Sending: '+ str(data))
            stdscr.refresh()
            sock.send(bytes(data))
            status = sock.recv(1)
            time.sleep(0.04)
            try:
                if not int.from_bytes(status, 'little'):
                    break
            except:
                break
    except ConnectionRefusedError:
        print("Error connecting to server")
    finally:
        sock.close()
        #Reset curses
        curses.nocbreak()
        stdscr.keypad(0)
        curses.echo()
        curses.endwin()
