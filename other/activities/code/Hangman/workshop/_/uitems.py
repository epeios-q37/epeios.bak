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

import educ

from accessor import *
from constants import *


# User items definitions.

def _defineUserFunction(name):
  return educ.defineUserItem(globals(),"uf",name)


def _defineUserVariable(name):
  return educ.defineUserItem(globals(),"uv",name)


def _defineUserClass(name):
  return educ.defineUserItem(globals(),"uc",name)


# 'list(…)' to avoid 'RuntimeError: dictionary changed size during iteration'.
for d in list(globals()):
  prefix = d[:3]
  if prefix == 'UF_':
    _defineUserFunction(globals()[d])
  elif prefix == 'UV_':
    _defineUserVariable(globals()[d])
  elif prefix == 'UC_':
    _defineUserClass(globals()[d])
