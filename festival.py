try:
    # module import
    from . import _festival
except (SystemError, ValueError):
    # local import
    import _festival

# taken from six. Don't want external dependencies
import sys
TEXT_TYPE = str if sys.version_info[0] == 3 else unicode

import subprocess
import tempfile
import os

def info():
    """Prints info about festival

    Prints information about the currently running
    festival instance, directly to stdout.
    Uses festival_banner() internally
    """

    _festival.info()

def textToWavFile(text):
    """Returns a filename for a wav file created
    with the text 'text'
    
    Use this method if you are concerned about removing
    the wav file once you have used it.
    Being given the filename, you are responsible for opening it
    as you see fit, and removing it once done
    """
    if not isinstance(text, TEXT_TYPE):
        text = text.decode('utf-8')
    
    return _festival._textToWav(text)

def textToWav(text):
    """Returns a file object for a wav file created with
    the text 'text'
    
    Note: it is your responsibility to remove the file when
    you are finished with it. It is automatically removed
    when you close the file.
    
    This method may return None if an error occurs
    """
    
    tmp_path = textToWavFile(text)
    try:
        with open(tmp_path, 'rb') as w:
            tmp_file = tempfile.SpooledTemporaryFile(max_size=4*1024*1024, mode='wb')
            tmp_file.write(w.read())
            tmp_file.seek(0)
        return tmp_file
    finally:
        os.remove(tmp_path)
    
    return None
    
def textToMp3(text):
    """Returns a file-like object (pipe) for an mp3 file
    created with the text 'text'
    
    Use Lame to encode the wav file to an mp3

    The file returned is a temp file. It is removed when closed
    """
    
    wav_file = textToWavFile(text)
    try:
        cmd = "lame --quiet -V 9 %s -" % wav_file
        tmp_file = tempfile.SpooledTemporaryFile(max_size=4*1024*1024, mode='wb')
        tmp_file.write(subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read())
        tmp_file.seek(0)
        return tmp_file
    finally:
        os.remove(wav_file)
    return None

def textToMp3File(text):
    """Returns a filename to an mp3 file created containing
    audio for the text 'text'

    """
    filename = textToWavFile(text)
    try:
        cmd = "lame --quiet -V9 %s %s.mp3" % (filename, filename)
        os.system(cmd)
    finally:
        os.remove(filename)
    return filename + ".mp3"

def sayText(text):
    """Tells _festival.to say the text 'text'
    
    Only works for machines with a sound card and audio device
    connected. May otherwise return errors

    Returns a boolean indicating if _festival.succeeded in saying
    the text
    """
    if not isinstance(text, TEXT_TYPE):
        text = text.decode('utf-8')
    return _festival._sayText(text)

def sayFile(filename):
    """Given a filename, say the contents of the file
    
    uses _festival.say_file() internally


    Returns a boolean indicating if _festival.succeeded in saying
    the text
    """

    if not os.path.exists(filename):
        raise ValueError("Could not find file %s" % filename);

    return _festival.sayFile(filename)

def setStretchFactor(f):
    """Set the stretch factor of the audio to be returned.
    
    f is a float between 0 and 5. A number less than 1 speeds up
    the voice (e.g. 0.5 doubles the speed). A number >1 slows down
    the voice (e.g. 2 halves the speed)
    """
    try:
        f = float(f)
    except ValueError:
        raise ValueError("Input parameter must be a float")
    return _festival.setStretchFactor(f)
    
def execCommand(cmd):
    """Execute a _festival.command
    
    Festival commands are written in lisp, and should
    be formatted correctly.
    
    E.g. execCommand("(SayText \"helo\")")
    """
    return _festival.execCommand(cmd)

