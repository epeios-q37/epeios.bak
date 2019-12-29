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

from workshop._._ import *


class Core(_.Core):
  def __init__(self,dom):
    _.Core.__init__(self,dom,ucHangman()())


def _reset(hangman,ufunction):
  suggestion = preBaseReset()
  secretWord = ufunction(hangman,suggestion,getRandomWord())
  postBaseReset(secretWord,None)


def _acConnect(core):
  show()
  _reset(core.userObject,ufAConnect())


def _acSubmit(core,dom,id):
  dom.addClass(id,"chosen")
  ufASubmit()(core.userObject,id.lower(),getBodyParts())



def _acRestart(core):
  _reset(core.userObject,ufARestart())


def main(callback,userFunctions,userFunctionLabels):
  mainBase(callback,globals(),
  (
    UC_HANGMAN,
    UF_ACONNECT,
    UF_ASUBMIT,
    UF_ARESTART,
  ),userFunctions,userFunctionLabels)
