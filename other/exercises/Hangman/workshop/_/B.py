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

import sys
sys.path.append("workshop/_")

import educ as _
from educ import Core

from workshop._._ import *

_REPORT_ANSWERS_HIDDEN = "ReportAnswersHidden"


def _reset():
  baseReset(None, getDictionary(), None, None)


def _append(list,item):
  list.append(item)


def _acConnect(core,dom):
  show("rk")
  _reset()

  
def _Submit(dom,letter,i18n):
  expected = rfIsLetterInWord(letter, getSecretWord())
  obtained = ufIsLetterInWord()(letter, getSecretWord())

  disabled = [_REPORT_ANSWERS_HIDDEN]
  enabled = []

  dom.setContent("Letter", letter.upper())

  _append(enabled if expected else disabled, "ExpectedTrue")
  _append(enabled if obtained else disabled, "ObtainedTrue")
  _append(enabled if expected==obtained else disabled, "ObtainedRight")

  dom.disableElements(disabled)
  dom.enableElements(enabled)


def _acSubmit(core, dom, id):
  _Submit(dom,id.lower(),getI18n())


def _acRestart(core, dom):
  _reset()
  dom.enableElement(_REPORT_ANSWERS_HIDDEN)


def main(callback, userFunctions, userFunctionLabels):
  mainBase(callback, globals(), (
    UF_PICK_WORD,
    UF_IS_LETTER_IN_WORD,
    ), userFunctions, userFunctionLabels)
