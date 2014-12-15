import _festival
import six
import os

def textToWav(text):
    if not isinstance(text, six.text_type):
        text = text.decode('utf-8')
    out_file = _festival._textToWav(text)
    return os.system("lame --quiet -V 9 %s -" % out_file)

def sayText(text):
    if not isinstance(text, six.text_type):
        text = text.decode('utf-8')
    _festival._sayText(text)

