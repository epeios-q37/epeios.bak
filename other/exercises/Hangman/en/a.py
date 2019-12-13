# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

# Don't forget.
from random import randint

"""
If missing or set to 'False', the text box displaying
the secret word, and allowing to type its value, 
will not be displayed (default mode).
"""
SHOW_SECRET_WORD = TRUE

DICTIONARY = ("tree", "house", "chair")

"""
The students will be asked to develop the
version 0, 1 and/or 2 of the '_pickWord(…)' function,
wether or not one wants to introduce tuples
(use of 'DICTIONARY').
"""

"""
Return a random word from 'DICTIONARY'.
"""
def _pickWord0():
  return DICTIONARY[randint(0, len(DICTIONARY)-1)]



"""
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise a random
word from 'DICTIONARY'.
"""
def _pickWord1(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONARY[randint(0, len(DICTIONARY)-1)]



"""
- 'dictionary': tuple containing words to pick,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise a random
word from 'dictionary'.
NOTA: the 'DICTIONARY' constant can naturally be removed.
"""
def _pickWord2(dictionary,suggestion):
  if suggestion:
    return suggestion
  else:
    return dictionary[randint(0, len(dictionary)-1)]


VERSION = 2 # 0, 1 or 2.

pickWord = globals()["_pickWord" + str(VERSION)]

go(globals())
