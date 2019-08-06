# coding: utf-8
""" 
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import sys, os

sys.path.append("./EduTK.python.zip")
sys.path.append("../EduTK.python.zip")

if ('EPEIOS_SRC' in os.environ):
  sys.path.append("/cygdrive/h/hg/epeios/other/libs/edutk/PYH/edutk")

from workshop._._ import *
import edutk as _

from random import randint

_CALLBACKS = "callbacks"

IS_LETTER_IN_WORD = "isLetterInWord"


def _getWord(dictionnary):
  return dictionnary[randint(0, len(dictionnary)-1)]


def _displayMask(word, guesses, debug):
  mask = ""
  for letter in word:
    if letter in guesses:
      mask += letter
    elif debug:
      mask += letter.upper()
    else:
      mask += "_"

  clearAndDisplay(mask)


def _reset(dictionnary,debug):
  redraw()
  resetHangman()
  setSecretWord( _getWord(dictionnary))
  print(getSecretWord())
  _displayMask(getSecretWord(),[],debug)


def _acConnect(core, dom, id):
  _reset(core.dictionnary,True)


def updateHanged_(hanged,errorsAmount):
  if errorsAmount:
    if errorsAmount <= len(hanged):
      drawFigure(hanged[errorsAmount-1])
    else:
      drawFigure(F_FACE)



def isLetterInWord_(letter, word):
  return letter in word

def _handleUserSolution(dom, hanged, letter, word):
  if (_.recall(_CALLBACKS)[IS_LETTER_IN_WORD](letter,word)):
    if (not letter in getGoodGuesses()):
      setGoodGuesses(getGoodGuesses() + letter)
      _displayMask(getSecretWord(),getGoodGuesses(),True)
  else:
    setErrorsAmount(getErrorsAmount() + 1)
    updateHanged_(hanged,getErrorsAmount())

def _acSubmit(core, dom, id):
  _handleUserSolution(dom, core.hanged,id.lower(), getSecretWord())


def _acRestart(core, dom):
  _reset(core.dictionnary,True)


def main(callback, callbacks):
  _.store(_CALLBACKS, callbacks),
  _.main(folder, callback, {
    "": _acConnect,
    "Submit": _acSubmit,
    "Restart": _acRestart
  },getAppTitle())
