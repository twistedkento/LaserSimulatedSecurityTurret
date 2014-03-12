import servo
import laser

serv = servo.servo()
manualmode = False

def turn_left():
	global serv
	serv.turn_left()
	
def turn_right():
	global serv
	serv.turn_right()

def get_angle():
	global serv
	return serv.get_angle()

def fire_laser():
    laser.fire()

def restart():
    print("Restarting software...")

def toggle_mode():
    if manualmode:
        manualmode = False
    else:
        manualmode = True
    Print("Manualmode: " + str(manualmode))