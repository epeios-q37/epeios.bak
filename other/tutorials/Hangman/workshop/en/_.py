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
 
import workshop._._ as workshop

class _Core(workshop.Core):
  i18n = {
    "Restart": "Restart"
  }

P_FACE = workshop.F_FACE
P_HEAD = workshop.F_HEAD
P_BODY = workshop.F_BODY
P_LEFT_ARM = workshop.F_LEFT_ARM
P_RIGHT_ARM = workshop.F_RIGHT_ARM
P_LEFT_LEG = workshop.F_LEFT_LEG
P_RIGHT_LEG = workshop.F_RIGHT_LEG

A_CONNECTION = workshop.A_CONNECT
A_GUESS = workshop.A_SUBMIT
A_RESTART = workshop.A_RESTART

def redraw():
  workshop.redraw()

def drawFigure(part):
  workshop.drawFigure(part)

def go(callback, callbacks):
  workshop.main(lambda dom: _Core(dom), callbacks, "Hangman game workshop", callback)


def erase():
  workshop.clear()


def display(text):
  workshop.display(text)


def eraseAndDisplay(text):
  workshop.clearAndDisplay(text)


def warn(text):
  workshop.alert(text)


def ask(text):
  return workshop.confirm(text)
