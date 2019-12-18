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

import sys, os, inspect
from random import randint
sys.path.append("workshop/_")

import educ as _
from accessor import *
from uitems import *

_I_OUTPUT = "Output"
_I_SECRET_WORD = "SecretWord"

_FOLDER = ""

_SHOW_SECRET_WORD = False

def redraw():
  _.dom().setLayout("", _.readBody(_FOLDER, getI18n()))


def _addItem(shortcut, items, label, ids):
  if shortcut in items:
    ids.append("Hide" + label)


def show(items="mgk"):
  ids = []
  items = items.lower()

  _addItem("m", items, "Mask", ids)
  _addItem("g", items, "Gallow", ids)
  _addItem("r", items, "Report", ids)
  _addItem("k", items, "Keyboard", ids)

  redraw()
  _.dom().disableElements(ids)

def drawBodyPart(part):
  _.dom().removeClass(part, "hidden")


def _createOutput(text)  :
  if not text.strip():
    text = " "   # &nbsp; => c2a0
  output = _.Atlas.createHTML()
  output.putTagAndValue("h1", text)
  return output


def display(text):
  _.dom().appendLayout(_I_OUTPUT, _createOutput(text))


def clearAndDisplay(text):
  _.dom().setLayout(_I_OUTPUT, _createOutput(text))


def alert(text):
  _.dom().alert(text)


def confirm(text):
  return _.dom().confirm(text)


def displayMask(word, guesses, fGetMask):
  clearAndDisplay(fGetMask()(word,guesses))


def showSecretWord():
  _.dom().removeAttribute(_I_SECRET_WORD, "style")


def _pickRandom(dictionary, suggestion):
  word = ""
  try:
    word = ufPickWord()(suggestion,dictionary[randint(0, len(dictionary)-1)])
  except TypeError:
    try:
      word = ufPickWord()(suggestion)
    except TypeError:
      word =  ufPickWord()()

  return word.lower()

  
def mainBaseReset(dictionary, suggestion):
  setErrorsAmount(0)
  setGoodGuesses("")

  secretWord = _pickRandom(dictionary, suggestion)
  setSecretWord(secretWord)

  return secretWord

def preBaseReset():
  suggestion = _.dom().getContent(_I_SECRET_WORD).strip()[:15] if _SHOW_SECRET_WORD else ""
  redraw()

  if _SHOW_SECRET_WORD:
    _.dom().disableElement("HideSecretWord")

  return suggestion


def postBaseReset(secretWord, fGetMask=ufGetMask):
  if _SHOW_SECRET_WORD:
    _.dom().setContent(_I_SECRET_WORD, secretWord)

  if fGetMask:
    displayMask(secretWord, "", fGetMask)


def baseReset(userObject, dictionary, fReset=ufReset, fGetMask=ufGetMask):
  suggestion = preBaseReset()

  if not fReset:
    fReset = lambda : mainBaseReset
  else:
    fGetMask = None

  secretWord = fReset()(userObject, dictionary, suggestion) if userObject else fReset()(dictionary, suggestion)

  postBaseReset(secretWord,fGetMask)


def _assignUserFunctions(labels, functions, names):
  return _.assignUserItems(labels, functions, names)


def mainBase(callback, globals, labels, userItems, userItemsNames):
# Uncomment for exceptions behaving normally again,
# instead of being displayed in a alert box.
#  _.useRegularExceptions()
  global _SHOW_SECRET_WORD
  _assignUserFunctions(labels, userItems, userItemsNames)
  if _.assignUserItem(UV_SHOW_SECRET_WORD, userItems, userItemsNames):
    _SHOW_SECRET_WORD = uvShowSecretWord()
  _.main(os.path.join("workshop", "assets", _FOLDER, "Head.html"), callback, {
     "": globals["_acConnect"],
    "Submit": globals["_acSubmit"],
    "Restart": globals["_acRestart"]
    }, getAppTitle())

