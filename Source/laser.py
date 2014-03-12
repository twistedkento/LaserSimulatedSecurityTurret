powerstate = False

def turn_on():
    global powerstate
    powerstate = True

def turn_off():
    global powerstate
    powerstate = False

def get_powerstate():
    return powerstate