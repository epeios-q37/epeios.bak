# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.f import *


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


"""
Let's introduce POO.
Class name must be 'Hangman',
but variables and methos name are free.
"""
class Hangman:
  def reset(self,dictionary):
    self.secretWord = pickWord(dictionary)
    self.goodGuesses = ""
    self.errorsAmount = 0

  def __init__(self):
    self.secretWord = ""
    self.goodGuesses = ""
    self.errorsAmount = 0

  """
  Updates the good guesses ot the amount of errors wether 'guess' is
  good ot not. Returns TRUE if 'guess' is good, FALSE otherwise.
  """
  def handleAndTestGuess(self, guess):
    if isLetterInWord(guess, self.secretWord):
      if not isLetterInWord(guess, self.goodGuesses):
        self.goodGuesses += guess
      return TRUE
    else:
      self.errorsAmount += 1
      return FALSE


"""
Same as previous exercise, but this time we use an object. 
"""
def reset(hangman,dictionary):
  hangman.reset(dictionary)
  print(hangman.secretWord)
  redraw()
  eraseAndDisplay(getMask(hangman.secretWord,""))

"""
Idem.
"""
def handleGuess(hangman,guess,parts):
  if hangman.handleAndTestGuess(guess):
    eraseAndDisplay(getMask(hangman.secretWord,hangman.goodGuesses))
  else:
    updateBody(parts, hangman.errorsAmount)

go(globals())
