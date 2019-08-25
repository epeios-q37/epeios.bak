# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.g import *

SHOW_SECRET_WORD = TRUE


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


Hangman = workshop.rcHangman


def reset(hangman,dictionary,suggestion):
  return workshop.rfReset(hangman,dictionary,suggestion)


def handleGuess(hangman,guess,parts):
  if hangman.handleAndTestGuess(guess):
    mask = getMask(hangman.secretWord, hangman.goodGuesses)
    eraseAndDisplay(mask)

    if not '_' in mask:
      report("You won! Congratulations!")
  else:
    if updateBody(parts, hangman.errorsAmount):
      report("\nYou lose!\nErrors: {}; good guesses: {}.\n\nThe secret word was: '{}'.".format(hangman.errorsAmount, len(hangman.goodGuesses), hangman.secretWord))


def AConnect(hangman,dictionary,suggestion):
  return reset(hangman,dictionary,suggestion)


def ASubmit(hangman,guess,parts):
  handleGuess(hangman,guess,parts)


def ARestart(hangman, dictionary, suggestion):
  return reset(hangman, dictionary, suggestion)



go(globals())
