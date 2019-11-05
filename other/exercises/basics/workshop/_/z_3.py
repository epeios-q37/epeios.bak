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
    def __init__(self,dom):
        _.Core.__init__(self,dom,_.ucHangman()())

_FOLDER = "z_3"

F_FACE = "Face"
F_HEAD = "Head"
F_BODY = "Body"
F_LEFT_ARM = "LeftArm"
F_RIGHT_ARM = "RightArm"
F_LEFT_LEG = "LeftLeg"
F_RIGHT_LEG = "RightLeg"


def _acConnect(core, dom, id):
    _.ufConnect()(core.userObject)


def _acSubmit(core, dom, id):
    dom.addClass(id, "chosen")

    _.ufSubmit()(core.userObject, id.lower())


def _acRestart(core, dom):
    _.ufRestart()(core.userObject)


def redraw():
    _.dom().setLayout("", _.readBody(_FOLDER, _.core().i18n))


def drawFigure(part):
    _.dom().removeClass(part, "hidden")


def main(globals,callback, userFunctionLabels, title):
    _.mainBase(_FOLDER, callback, {
        "": _acConnect,
        "Submit": _acSubmit,
        "Restart": _acRestart
    }, (_.F_CONNECT, _.F_SUBMIT, _.F_RESTART, _.C_HANGMAN), globals, userFunctionLabels, title)
