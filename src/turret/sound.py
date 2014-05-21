import subprocess
import os

class Sound(object):
    '''
        Class that handles sound
    '''
    
    def __init__(self):
        from os.path import expanduser
        self.path = expanduser('~')
        self.sounds = dict()
        self.load_sounds()
    
    def play_sound(self, name):
        self.player = subprocess.Popen(["mplayer", self.path + self.sounds[name]], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
        
    def load_sounds(self):
        for file in os.listdir(self.path):
            if file.endswith(".wav") or file.endswith(".mp3"):
                self.sounds[file[:file.rfind('.')]] = file


