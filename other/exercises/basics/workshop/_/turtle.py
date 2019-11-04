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
 
import math, colorsys
import workshop._._ as _

import sys, os

sys.path.append("./Tortoise.zip")
sys.path.append("../Tortoise.zip")

if ('EPEIOS_SRC' in os.environ):
  sys.path.append("/cygdrive/h/hg/epeios/other/libs/tortoise/PYH/tortoise")

import tortoise

_TURTLE = "turtle"

def init( dom = None ):
  _.store(_TURTLE, tortoise.Tortoise(dom,"SVG"))

def _turtle():
  return _.recall(_TURTLE)

def up():
  _turtle().up()

def down():
  _turtle().down()

def forward(distance):
  _turtle().forward( distance)

def setColorRGB(r,g,b): # 0 to 255
  _turtle().setColorRGB(r,g,b)

def setColorHSL(h,s,l): # h: 0-360, s & l: 0-100 (%)
  _turtle().setColorHSL(h,s,l)

def right(angle):
  _turtle().right(angle)

def left(angle):
  _turtle().left(angle)

def draw(dom = None):
  _turtle().draw( dom )

def clear(dom = None):
  _turtle().clear( dom )
