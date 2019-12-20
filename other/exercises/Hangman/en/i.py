# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.i import *

SHOW_SECRET_WORD = TRUE


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


"""
At first, don't handle the 'inProgress' variable
member from the 'Hangman' class.
"""


"""
Add the handling of the 'inProgress' variable.
Only the 'reset(…)' and the '__init__(…)'
methods are concerned.
"""
class Hangman:
  def reset(self, dictionary, suggestion):
    self.secretWord = pickWord(dictionary, suggestion)
    self.goodGuesses = ""
    self.errorsAmount = 0
    self.inProgress = TRUE

  def __init__(self):
    self.secretWord = ""
    self.goodGuesses = ""
    self.errorsAmount = 0
    self.inProgress = FALSE

  def handleAndTestGuess(self, guess):
    if isLetterInWord(guess, self.secretWord):
      if not isLetterInWord(guess, self.goodGuesses):
        self.goodGuesses += guess
      return TRUE
    else:
      self.errorsAmount += 1
      return FALSE


"""
Add the testing.
"""
def getMaskAndTestIfHasWon(word, guesses):
  mask = ""
  hasWon = True

  for letter in word:
    if isLetterInWord(letter, guesses):
      mask += letter
    else:
      mask += "_"
      hasWon = False

  return mask, hasWon


"""
Add the testing.
"""
def updateBodyAndTestIfHasLost(parts, errorsAmount):
  if errorsAmount <= len(parts):
    drawBodyPart(parts[errorsAmount-1])

  if errorsAmount >= len(parts):
    drawBodyPart(P_FACE)
    return True
  else:
    return False


"""
Add the notifications.
"""
def handleGuess(hangman,guess,parts):
  if hangman.handleAndTestGuess(guess):
    mask,hasWon = getMaskAndTestIfHasWon(hangman.secretWord, hangman.goodGuesses)
    eraseAndDisplay(mask)
    if hasWon and hangman.inProgress:
      notify("You won! Congratulations!")
      hangman.inProgress = FALSE
  elif hangman.inProgress and updateBodyAndTestIfHasLost(parts, hangman.errorsAmount):
    notify("\nYou lose!\nErrors: {}; good guesses: {}.\n\nThe secret word was: '{}'.".format(hangman.errorsAmount, len(hangman.goodGuesses), hangman.secretWord))
    hangman.inProgress = FALSE

"""
Modify to use 'getMaskAndTestIfHasWon(…)'.
"""
def reset(hangman, dictionary, suggestion):
  hangman.reset(dictionary, suggestion)
  print(hangman.secretWord)
  eraseAndDisplay(getMaskAndTestIfHasWon(hangman.secretWord, "")[0])

  return hangman.secretWord


"""
Called on new connection. 
"""
def AConnect(hangman,dictionary,suggestion):
  return reset(hangman,dictionary,suggestion)


"""
Called on new guess.
NOTA: the letter will be disabled on the keyboard. 
"""
def ASubmit(hangman, guess, parts):
  handleGuess(hangman,guess,parts)

"""
Called on a click on the 'Restart' button.
"""
def ARestart(hangman, dictionary, suggestion):
  if hangman.inProgress:
    notify("\nErrors: {}; good guesses: {}.\n\nThe secret word was: '{}'.".format(
        hangman.errorsAmount, len(hangman.goodGuesses), hangman.secretWord))

  return reset(hangman, dictionary, suggestion)


go(globals())
