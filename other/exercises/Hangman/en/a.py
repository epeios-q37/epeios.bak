# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

# Don't forget.
from random import randint

DICTIONARY = ("tree", "house", "chair")


"""
The students will be asked to develop the
version 0, 1 and/or 2 of the '_PickWord(…)' function,
wether or not one wants to introduce tuples
('DICTIONARY').
"""

"""
Returns a random word from 'DICTIONNARY'.
"""
def _pickWord0():
  return DICTIONARY[randint(0, len(DICTIONARY)-1)]



"""
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Returns 'suggestion', if not empty, otherwise a random
word from 'DICTIONNARY'.
"""
def _pickWord1(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONARY[randint(0, len(DICTIONARY)-1)]



"""
- 'dictionnary': tuple containing words to pick,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Returns 'suggestion', if not empty, otherwise a random
word from 'dictionary'.
NOTA: the 'DICTIONNARY' constant can naturally be removed.
"""
def _pickWord2(dictionary,suggestion):
  if suggestion:
    return suggestion
  else:
    return dictionary[randint(0, len(dictionary)-1)]


VERSION = 2 # 0, 1 or 2.

pickWord = globals()["_pickWord" + str(VERSION)]

go(globals())
