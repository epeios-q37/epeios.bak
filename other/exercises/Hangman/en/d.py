# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.d import *

# Don't forget.
from random import randint

def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)

"""
- 'dictionnary': tuple containing words to pick,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Returns 'suggestion', if not empty, otherwise a random
word from 'dictionary'.
"""
def pickWord(dictionary,suggestion):
  if suggestion:
    return suggestion
  else:
    return dictionary[randint(0, len(dictionary)-1)]


go(globals())
