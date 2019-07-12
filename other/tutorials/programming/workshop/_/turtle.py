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

_POS_X = "posx"
_POS_Y = "posy"
_ANGLE = "angle"
_COLOR = "color"
_PATH = "path"

_CENTRE_X = 150
_CENTRE_Y = 150

def init():
  _.store(_POS_X,_CENTRE_X)
  _.store(_POS_Y,_CENTRE_Y)
  _.store(_ANGLE, 0)
  _.store(_COLOR,(0,0,0))
  _.store(_PATH,_.Atlas.createHTML())

def _round(value):
  return int(math.floor(value + 0.5))

def _push(motion):
    path = _.recall(_PATH)
    path.pushTag("path")
    path.putAttribute("stroke", "rgb({}, {}, {})".format(*_.recall(_COLOR)))
    path.putAttribute("stroke-width", "2")
    path.putAttribute("fill", "none")
    path.putAttribute('d', motion)
    path.popTag()

def forward(distance):
  angle = _.recall(_ANGLE)
  posx = _round(_.recall(_POS_X) + distance * math.sin(angle))
  posy = _round(_.recall(_POS_Y) - distance * math.cos(angle))

  _push("M {} {} L {} {}".format(_.recall('posx'), _.recall('posy'), posx, posy))

  _.store('posx', posx)
  _.store('posy', posy)

def setColorRGB(r,g,b): # 0 to 255
  _.store(_COLOR,(r,g,b))

def setColorHSL(h,s,l): # h: 0-360, s & l: 0-100 (%)
  # _.store(_COLOR,tuple(_round(255*x) for x in colorsys.hls_to_rgb(h/360,l/100,s/100))) With Python 2, calculation return integer instaed of float…
    _.store(_COLOR,tuple(_round(255*x) for x in colorsys.hls_to_rgb(h/360.0,l/100.0,s/100.0)))

def right(angle):
  _.store(_ANGLE,math.radians((math.degrees(_.recall(_ANGLE)) + angle ) % 360))

def left(angle):
  right(360 - angle)

def draw(dom):
  dom.setLayout("SVG", _.recall(_PATH))
  _.store(_PATH,_.Atlas.createHTML())
  _.store(_POS_X,_CENTRE_X)
  _.store(_POS_Y,_CENTRE_Y)

