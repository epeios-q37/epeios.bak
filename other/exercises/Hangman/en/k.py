# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.j import *

DISCLOSE_SECRET_WORD = TRUE


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
  return workshop.rfIsLetterInWord(*args)


def getMask(*args):
  return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


"""
Let's introduce object-oriented programmation.
Class name must be 'Hangman',
but variables and methods name are free.
"""
class Hangman:
  def reset(self,suggestion,randomWord):
    self.secretWord = pickWord(suggestion,randomWord)
    self.goodGuesses = ""
    self.errorsAmount = 0

  def __init__(self):
    self.secretWord = ""
    self.goodGuesses = ""
    self.errorsAmount = 0

  """
  Update the good guesses ot the amount of errors wether 'guess' is
  good ot not. Return TRUE if 'guess' is good, FALSE otherwise.
  """
  def handleAndTestGuess(self,guess):
    if isLetterInWord(guess,self.secretWord):
      if not isLetterInWord(guess,self.goodGuesses):
        self.goodGuesses += guess
      return TRUE
    else:
      self.errorsAmount += 1
      return FALSE


"""
Same as previous exercise, but this time we use an object. 
"""
def reset(hangman,suggestion,randomWord):
  hangman.reset(suggestion,randomWord)
  print(hangman.secretWord)
  eraseAndDisplay(getMask(hangman.secretWord,""))

  if DISCLOSE_SECRET_WORD:
    discloseSecretWord(hangman.secretWord,"")


"""
Idem.
"""
def handleGuess(hangman,guess):
  if hangman.handleAndTestGuess(guess):
    eraseAndDisplay(getMask(hangman.secretWord,hangman.goodGuesses))
  else:
    updateBody(hangman.errorsAmount)

go(globals())
