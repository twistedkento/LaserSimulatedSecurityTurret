scanning = False

def start_scanning():
    global scanning
    scanning = True

def stop_scanning():
    global scanning
    scanning = False

def get_state():
    global scanning
    return scanning