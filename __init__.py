from . import festival
import six
import subprocess

def textToWav(text):
    if not isinstance(text, six.text_type):
        text = text.decode('utf-8')
    wav_file = festival._textToWav(text)
    cmd = "lame --quiet -V 9 %s -" % wav_file
    return subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)

def sayText(text):
    if not isinstance(text, six.text_type):
        text = text.decode('utf-8')
    festival._sayText(text)

def setStretchFactor(f):
    try:
        f = float(f)
    except ValueError:
        raise ValueError("Input parameter must be a float")
    festival.setStretchFactor(f)
    
def execCommand(cmd):
    festival.execCommand(cmd)
