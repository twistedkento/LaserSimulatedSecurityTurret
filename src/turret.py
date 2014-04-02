import servo
import laser
import camera

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
	
def run():
	if camera.visible_target():
		fire_laser()
			
def start(time):
	time = time * 100
	while(time > 0):
		time-=1
		run()
		