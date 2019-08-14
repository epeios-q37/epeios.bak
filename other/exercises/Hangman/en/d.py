# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.d import *

"""
Some variables are now handled by the student. Names are free.
Can be omitted, as 'reset(…)' will be call before thie variables
will be used.
"""
goodGuesses = ""
errorsAmount = 0

"""
NOTA: the three folowing functions are not called outside this file.
"""
def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)

"""
Must reset the variables for a new game.
"""
def reset():
  global goodGuesses, errorsAmount

  goodGuesses = ""
  errorsAmount = 0

"""
- 'guess': the letter choosen by the player,
- 'word': the secret word,
- 'parts': the different part of the body to draw.
If 'guess' in 'word', must update the mask, otherwise
must update the drawing of the body.
"""
def handleGuess(guess,word,parts):
  global goodGuesses, errorsAmount

  if isLetterInWord(guess, word): # Test is not mandatory
    if not isLetterInWord(guess,goodGuesses):
      goodGuesses += guess
      eraseAndDisplay(getMask(word,goodGuesses))
  else:
    errorsAmount += 1
    updateBody(parts, errorsAmount)

go(globals())
