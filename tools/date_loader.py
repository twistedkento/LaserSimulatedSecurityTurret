#!/usr/bin/python
'''DateFixer 9000 (Don't use it on E-harmony!)'''

import time
import os
import sys
from subprocess import call

'''Static settings. Change if needed.'''
HOME_DIR = os.path.expanduser("~")
CONFIG_DIR = HOME_DIR + '/.config/turret_config/'
CONFIG_FILE = CONFIG_DIR + 'turret.conf'
STATIC_DATE = '040210002014'

def get_system_date():
    '''Gets the current date from the system or the predefined static date
    if it is bigger.'''
    return max_date(time.strftime('%m%d%H%M%Y'), STATIC_DATE)

def get_date_from_fs():
    '''Gets the current date from the filesystem or the predefined static date
    if it is bigger.'''
    try:
        with open(CONFIG_FILE) as f:
            return max_date(f.read().strip('\n'), STATIC_DATE)
    except:
        return STATIC_DATE

def max_date(*indata):
    '''Returns the biggest date given.'''
    try:
        return sorted(indata, key=lambda x: time.strptime(x, '%m%d%H%M%Y'))[-1]
    except ValueError:
        return STATIC_DATE

def save_date_to_fs():
    '''Saves the latest date to a cache file.'''
    date = get_latest_date()
    with open(CONFIG_FILE, 'w') as f:
        f.write(date)

def get_latest_date():
    '''Gets the latest date from either the system or the saved file.'''
    return max_date(get_system_date(), get_date_from_fs())

def set_system_date():
    '''Calls the posix command date with the latest date found by the application.'''
    if call(['date', get_latest_date()]) != 0:
        raise OSError("Could not set system date!")

def check_fs():
    '''Checks if the save directory exists and that it is a valid folder. If it does not exists then create it.'''
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
    elif '--save-date' in sys.argv:
        save_date_to_fs()
    else:
        print('Usage: ' + sys.argv[0] + ' [option]')
        print('   --load-date'.ljust(40) + 'Loads the date stored in the filesystem and sets it as current date.')
        print('   --save-date'.ljust(40) + 'Saves the date used by the filesystem(if it\'s a later date then in the savefile.')

