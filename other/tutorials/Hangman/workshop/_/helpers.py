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

import sys
sys.path.append("workshop/_")

import educ as _
from accessor import *
from ufunctions import *

from random import randint

_OUTPUT = "output"

_FOLDER = ""

def redraw():
  _.dom().setLayout("", _.readBody(_FOLDER, _.core().i18n))


def drawBodyPart(part):
  _.dom().removeClass(part, "hidden")


def display(text):
  output = _.Atlas.createHTML()
  output.putTagAndValue("h1", text)
  _.dom().appendLayout(_OUTPUT, output)


def clearAndDisplay(text):
  output = _.Atlas.createHTML()
  output.putTagAndValue("h1", text)
  _.dom().setLayout(_OUTPUT, output)


def alert(text):
  _.dom().alert(text)


def confirm(text):
  return _.dom().confirm(text)


def displayMask(word, guesses, dev, fGetMask=ufGetMask):
  clearAndDisplay(fGetMask(word,guesses,dev))


def resetBase(dictionnary, dev, fGetMask = ufGetMask):
  redraw()
  resetHangman()
  setSecretWord(getWord(dictionnary))
  print(getSecretWord())
  displayMask(getSecretWord(), "", dev, fGetMask)


def _setUserFunctions(ids, functions, labels):
  links = {}

  for id in ids:
    label = labels[id]

    if not label in functions:
      raise NameError("Missing '{}' function.".format(label) )

    links[id] = functions[label]

  setUserFunctions(links)


def mainBase(callback, globals, ids, userFunctions, userFunctionLabels):
  _setUserFunctions(ids, userFunctions, userFunctionLabels)
  _.main(_FOLDER, callback, {
     "": globals["_acConnect"],
    "Submit": globals["_acSubmit"],
    "Restart": globals["_acRestart"]
    }, getAppTitle())


def resetHangman():
  setErrorsAmount( 0 )
  setGoodGuesses( "" )
  setSecretWord( "" )


def getWord(dictionnary):
  return dictionnary[randint(0, len(dictionnary)-1)]

