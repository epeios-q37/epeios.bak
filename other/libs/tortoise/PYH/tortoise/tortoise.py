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

import atlastk
import math, colorsys, random

_S_UP = True
_S_DOWN = False


def _round(value):
  return int(math.floor(value + 0.5))


class Tortoise:
    def _reset(self):
        self._posx = 0
        self._posy = 0
        self._angle = 0
        self._color = (0, 0, 0)
        self._autoDrawCount = 0
        self._state = _S_UP
        self._path = atlastk.createHTML()

    def __init__(self, dom, id):
        self._id = id   # id of the SVG element.
        self._dom = dom
        self._autoDraw = 0 if dom == None else 1
        self._reset()

    def _draw(self, dom = None):
        if dom == None:
            dom = self._dom

        dom.appendLayout(self._id, self._path)
        self._path = atlastk.createHTML()
        self._autoDrawCount = 0
        dom.flush()

    def _clear(self, dom):
        if dom == None:
            dom = self._dom

        self._reset()
        dom.setLayout(self._id, atlastk.createHTML())

    def _push(self, x1, y1, x2, y2):
        self._path.pushTag("path")
        self._path.putAttribute(
            "stroke", "rgb({}, {}, {})".format(*self._color))
        self._path.putAttribute("stroke-width", "1")
        self._path.putAttribute("fill", "none")
        self._path.putAttribute('d', "M {} {} L {} {}".format(int(x1), int(y1), int(x2), int(y2)))
        self._path.popTag()

        if self._autoDraw:
            self._autoDrawCount += 1

            if self._autoDrawCount >= self._autoDraw:
                self._draw()

    def getAngle(self):
        return math.degrees(self._angle)

    def up(self):
        self._state = _S_UP

    def down(self):
        self._state = _S_DOWN

    def setAutoDraw(self,value):
        self._autoDraw = value

    def setPosition(self,x,y):
        self._posx = x
        self._posy = y

    def forward(self, distance):
        posx = self._posx + distance * math.sin(self._angle)
        posy = _round(self._posy) - distance * math.cos(self._angle)

        if self._state == _S_DOWN:
            self._push(self._posx, self._posy, posx, posy)

        self._posx = posx
        self._posy = posy

    def setColorRGB(self, r, g, b):  # 0 to 255
        self._color = (r, g, b)

    def setColorHSL(self, h, s, l):  # h: 0-360, s & l: 0-100 (%)
        # '….0',  or, with Python 2, calculations return integer instead of float…
        self._color = tuple(_round(255*x)
                          for x in colorsys.hls_to_rgb(h/360.0, l/100.0, s/100.0))

    def right(self, angle):
        self._angle = math.radians((math.degrees(self._angle) + angle) % 360)

    def left(self, angle):
        self.right(360 - angle)

    def draw(self, dom = None):
        self._draw(dom)

    def clear(self, dom = None):
        self._clear(dom)

