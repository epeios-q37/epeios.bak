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

def _reset(dictionnary,hangman):
  redraw()
  hangman.reset()
  hangman.secretWord = _getWord(dictionnary)
  print(hangman.secretWord)
  display(hangman.secretWord)


def _acConnect(core, dom, id):
  _reset(core.dictionnary, core.userObject)


def isLetterInWord_(word, letter):
  return letter in word


def _testUserSolution(dom, word, letter):
  u = _.recall(_CALLBACKS)[IS_LETTER_IN_WORD](word, letter)
  s = isLetterInWord_(word, letter)

  print (word, letter, s, u, s == u)


def _acSubmit(core, dom, id):
  _testUserSolution(dom,core.userObject.secretWord,id.lower())


def _acRestart(core, dom):
  _reset(core.dictionnary, core.userObject)


def main(callback, callbacks, title, userCallback):
  _.store(_CALLBACKS, callbacks),
  _.main(folder, callback, {
    "": _acConnect,
    "Submit": _acSubmit,
    "Restart": _acRestart
  }, title, userCallback)
