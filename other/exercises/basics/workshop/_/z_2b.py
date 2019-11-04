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
 
import math
import workshop._._ as _
import workshop._.turtle as turtle

_FOLDER = "z_2b"

def _acConnect(core,dom,id):
  dom.setLayout("",_.readBody(_FOLDER, core.i18n))

def _acDraw(core,dom):
  turtle.clear(dom)
  (polygons, segments) = dom.getContents(("polygons","segments")).values()
  _.ufMyFunction()(polygons, segments)
  turtle.draw(dom)


def main(globals,callback, userFunctionLabels,title):
  _.mainBase(_FOLDER, callback, {
      "" : _acConnect,
      "Draw" : _acDraw,
    }, (_.F_MY_FUNCTION,), globals, userFunctionLabels, title )

turtle.init()
