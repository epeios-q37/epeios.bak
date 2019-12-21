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
from educ import Core

from workshop._._ import *


def _reset():
  baseReset(None, getDictionary(), None)


def _acConnect(core, dom):
  show("mk")
  _reset()


def _Submit(letter):
  if ufIsLetterInWord()(letter, getSecretWord()):
    if (not letter in getGoodGuesses()):
      setGoodGuesses(getGoodGuesses() + letter)
      displayMask(getSecretWord(), getGoodGuesses(), lambda: ufGetMask())
  else:
    setErrorsAmount(getErrorsAmount() + 1)
    try:
      rfUpdateBody(getBodyParts(), getErrorsAmount())
    


def _acSubmit(core, dom, id):
  _Submit(id.lower())


def _acRestart():
  _reset()


def main(callback, userFunctions, userFunctionLabels):
  mainBase(callback, globals(),
  (
    UF_PICK_WORD,
    UF_IS_LETTER_IN_WORD,
    UF_GET_MASK
  ), userFunctions, userFunctionLabels)
