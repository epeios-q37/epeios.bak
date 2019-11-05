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

_FOLDER = "c"

def _acConnect(c, dom):
    dom.setLayout("", _.readBody(_FOLDER, c.bodyI18n))
    dom.focus("input")


def _acSubmit(c, dom):
    input=dom.getContent("input").strip()

    if (len(input)) != 0:
        _.ufSalute()(dom.getContent("input"))
        dom.setContent( "input", "")
        dom.removeClass("output", "hidden")
    else:
        dom.alert(c.i18n["NameIsMandatory"])
        dom.setContent("input", "")
        dom.focus("input")


def main(callback, globals, userFunctionLabels, title):
  _.mainBase(_FOLDER, callback,
  {
    "": _acConnect,
    "Submit": _acSubmit,
  }, (_.F_SALUTE,), globals, userFunctionLabels, title)
