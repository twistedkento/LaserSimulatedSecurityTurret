import servo

serv = servo.servo()

def turn_left():
	global serv
	serv.turn_left()
	
def turn_right():
	global serv
	serv.turn_right()

def get_angle():
	global serv
	return serv.get_angle()