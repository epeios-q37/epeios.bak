# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *

# Don't forget.
from random import randint

"""
If missing or set to 'False', the text box displaying
the secret word, and allowing to type its value, 
will not be displayed (default mode).
"""
SHOW_SECRET_WORD = TRUE

# To define only with version 4 of the '_pickWord()' function.
DICTIONARY = ("tree", "house", "chair")


"""
Version 1:
Returns a random word from 'DICTIONARY'.
"""
def _pickWord1():
  return "foot"


"""
Version 2:
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise some word.
"""
def _pickWord2(suggestion):
  if suggestion:
    return suggestion
  else:
    return "arm"


"""
As next the exercise, one can choose to make them develop:
- version 3 only, if you do not want to deal with tuples;
- version 3, then version 4;
- directly the version 4.
"""


"""
Version 3:
- 'randomWord': an random word,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise 'randomWord'.
"""
def _pickWord3(suggestion,randomWord):
  if suggestion:
    return suggestion
  else:
    return randomWord


"""
Version 4:
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise a random word.
"""
def _pickWord4(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONARY[randint(0, len(DICTIONARY)-1)]


VERSION = 4 # 1, 2, 3 or 2.

pickWord = globals()["_pickWord" + str(VERSION)]

go(globals())
