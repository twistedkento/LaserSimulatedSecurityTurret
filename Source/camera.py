scanning = False

def start_scanning():
    global scanning
    scanning = True
    print("Camera starts scanning")

def stop_scanning():
    global scanning
    scanning = False
    print("Camera stops scanning")

def get_state():
    global scanning
    return scanning