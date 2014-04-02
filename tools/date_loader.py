import time
import os
import sys
from subprocess import call

HOME_DIR = os.path.expanduser("~")
CONFIG_DIR = HOME_DIR + '/.config/turret_config/'
CONFIG_FILE = CONFIG_DIR + 'turret.conf'
STATIC_DATE = '040210002014'

def get_system_date():
    return max_date(time.strftime('%m%d%H%M%Y'), STATIC_DATE)

def get_date_from_fs():
    try:
        with open(CONFIG_FILE) as f:
            return max_date(f.read().strip('\n'), STATIC_DATE)
    except:
        return STATIC_DATE

def max_date(*indata):
    try:
        return sorted(indata, key=lambda x: time.strptime(x, '%m%d%H%M%Y'))[-1]
    except ValueError:
        return STATIC_DATE

def save_date_to_fs():
    date = get_latest_date()
    with open(CONFIG_FILE, 'w') as f:
        f.write(date)

def get_latest_date():
    return max_date(get_system_date(), get_date_from_fs())

def set_system_date():
    if call(['date', get_latest_date()]) != 0:
        raise OSError("Could not set system date!")

def check_fs():
    if not os.path.lexists(os.path.abspath(CONFIG_DIR)):
        sys.stderr.write('Config directory does not exists creating: ' + CONFIG_DIR + '\n')
        sys.stderr.flush()
        os.mkdir(CONFIG_DIR)
    elif os.path.isfile(os.path.abspath(CONFIG_DIR)):
        sys.stderr.write('Error the config folder path points to a file!\nCRASH AND BURN!\n')
        sys.stderr.flush()
        sys.exit(1)

if __name__ == '__main__':
    check_fs()
    if '--load-date' in sys.argv:
        set_system_date()
    if '--save-date' in sys.argv:
        save_date_to_fs()
