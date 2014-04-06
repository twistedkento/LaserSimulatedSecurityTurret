#!/usr/bin/env python

'''
    File: date_loader.py
    Author: Kenny Nilsson
    Email: kenny@kento.se
    Github: https://github.com/twistedretard
    Description: Saving and restoring the date on Arch Linux ARM
'''

import time
import os
import sys
from subprocess import call

# Static settings. Change if needed.
__home_dir = os.path.expanduser("~")
__config_dir = __home_dir + '/.config'
__cache_dir = __config_dir + '/turret_config/'
__cache_file = __cache_dir + 'turret.conf'
__static_date = '040210002014'

def get_system_date():
    '''
        Gets the current date from the system or the predefined static date
        if it is later.
    '''
    return max_date(time.strftime('%m%d%H%M%Y'), __static_date)

def get_date_from_fs():
    '''
        Gets the current date from the filesystem or the predefined static date
        if it is later.
    '''
    try:
        with open(__cache_file) as f:
            return max_date(f.read().strip('\n'), __static_date)
    except:
        return __static_date

def max_date(*indata):
    '''
        Returns the latest date given.
    '''
    try:
        return sorted(indata, key=lambda x: time.strptime(x, '%m%d%H%M%Y'),reverse=True)[0]
    except ValueError:
        return __static_date

def save_date_to_fs():
    '''
        Saves the latest date to a cache file.
    '''
    date = get_latest_date()
    with open(__cache_file, 'w') as f:
        f.write(date)

def get_latest_date():
    '''
        Gets the latest date from either the system or the saved file.
    '''
    return max_date(get_system_date(), get_date_from_fs())

def set_system_date():
    '''
        Calls the posix command date with the latest date found by the application.
    '''
    if call(['date', get_latest_date()]) != 0:
        raise OSError("Could not set system date!")

def print_error(text):
    '''
        A simple print method that outputs to stderr if the debugflag is set.
    '''
    if sys.flags.debug:
        sys.stderr.write(text)
        sys.stderr.flush()

def check_fs():
    '''
        Checks if the save directory exists and that it is a valid folder.
        Create the folder if it does not exist.
    '''
    if not os.path.lexists(os.path.abspath(__config_dir)):
        print_error('Config directory does not exists creating: ' + __config_dir + '\n')
        os.mkdir(__config_dir)
    if not os.path.lexists(os.path.abspath(__cache_dir)):
        print_error('Cache directory does not exists creating: ' + __cache_dir + '\n')
        os.mkdir(__cache_dir)
    elif os.path.isfile(os.path.abspath(__cache_dir)):
        print_error('Error the config folder path points to a file!\nCRASH AND BURN!\n')
        sys.exit(1)

def print_commands():
    '''
        Prints the command line arguments avaible for this script
    '''
    print('\nUsage: ' + sys.argv[0] + ' [option]',end='\n\n')
    print('   --load-date'.ljust(40) + 'Loads the date stored in the filesystem and'    \
            + ' sets it as current date.')
    print('   --save-date'.ljust(40) + 'Saves the date used by the operatingsystem if it\'s' \
            + ' a later date then the on in the savefile.',end='\n\n')

    
if __name__ == '__main__':
    check_fs()
    if '--load-date' in sys.argv:
        set_system_date()
    elif '--save-date' in sys.argv:
        save_date_to_fs()
    else:
        print_commands()
