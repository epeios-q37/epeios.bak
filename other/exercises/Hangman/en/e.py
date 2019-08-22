# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.e import *

SHOW_SECRET_WORD = TRUE

"""
Some variables are now handled by the student. Names are free.
Can be omitted, as 'reset(…)' will be call before thie variables
will be used.
"""
secretWord = ""
goodGuesses = ""
errorsAmount = 0

"""
NOTA: the four folowing functions are not called outside this file.
"""

def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


"""
Resets the variables and the display for a new round.
Returns the secret word.
"""
def reset(dictionary, suggestion):
  global secretWord, goodGuesses, errorsAmount

  secretWord = pickWord(dictionary, suggestion)
  goodGuesses = ""
  errorsAmount = 0
  print(secretWord)
  eraseAndDisplay(getMask(secretWord,""))

  return secretWord

"""
N.B.: NOT THREAD-SAFE!!!
- 'guess': the letter choosen by the player,
- 'parts': the different part of the body to draw.
If 'guess' in 'word', must update the mask, otherwise
must update the drawing of the body.
"""
def handleGuess(guess,parts):
  global goodGuesses, errorsAmount

  if isLetterInWord(guess, secretWord): # Test is not mandatory
    if not isLetterInWord(guess,goodGuesses):
      goodGuesses += guess
      eraseAndDisplay(getMask(secretWord,goodGuesses))
  else:
    errorsAmount += 1
    updateBody(parts, errorsAmount)

go(globals())
