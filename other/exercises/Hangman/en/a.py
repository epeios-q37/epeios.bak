# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *


"""
If missing or set to 'False', the text box displaying
the secret word, and allowing to type its value, 
will not be displayed (default mode).
"""
DISCLOSE_SECRET_WORD = TRUE


"""
Returns a random word from 'DICTIONARY'.
"""
def pickWord():
  return "foot"


go(globals())
