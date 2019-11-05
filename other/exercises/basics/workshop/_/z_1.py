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
 
import workshop._._ as _

class Core(_.Core):
  pass

_FOLDER = "z_1"

def _acConnect(d,dom,id):
  dom.setLayout("",_.readBody(_FOLDER,d.bodyI18n))
  dom.focus("a")


def _test(value):
  try:
    number = float(value)
    return number <100 and number >-100
  except:
    return False


def _test(value):
  number = float(value)

  if number <-100 or number > 100:
    raise ValueError("Out of range: " + value)

  return number
  
def _acSubmit(core,dom,id):
  [a,b,c,operator] = dom.getContents(['a','b','c','operator']).values()

  try:
    a = _test(a)
    b = _test(b)
    c = _test(c)
  except Exception as e:
    if _reportErrors:
      dom.alert(core.i18n["BadValue"])
    else:
      raise e
  else:
    if a == 0 and _reportErrors:
      dom.alert(core.i18n["ACannotBe0"])
    else:
      _.ufResolve()(a, b, c, operator)
      dom.disableElement("Hide")

  dom.focus('a')


def main(globals,callback,userFunctionLabels,title,patchErrorVariableName):
  global _reportErrors
  _reportErrors = globals[patchErrorVariableName]

  _.mainBase(_FOLDER, callback, {
      "" : _acConnect,
      "Submit": _acSubmit
  }, (_.F_RESOLVE,), globals, userFunctionLabels, title)
