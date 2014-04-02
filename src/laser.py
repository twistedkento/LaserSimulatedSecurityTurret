powerstate = False

def turn_on():
    global powerstate
    powerstate = True
    print("Powerstate: " + str(powerstate))

def turn_off():
    global powerstate
    powerstate = False
    print("Powerstate: " + str(powerstate))

def get_powerstate():
    return powerstate
    
def fire():
    print("IMMA FIRIN' MA LAZOR!!!")