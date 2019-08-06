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
  i18n = {
    "Restart": "Restart"
  }

P_FACE = _.F_FACE
P_HEAD = _.F_HEAD
P_BODY = _.F_BODY
P_LEFT_ARM = _.F_LEFT_ARM
P_RIGHT_ARM = _.F_RIGHT_ARM
P_LEFT_LEG = _.F_LEFT_LEG
P_RIGHT_LEG = _.F_RIGHT_LEG

A_CONNECTION = _.A_CONNECT
A_GUESS = _.A_SUBMIT
A_RESTART = _.A_RESTART

DICTIONARY = [
    "accommodate", "afterthought", "allegiance", "aloft", "ancestor", "anticipation", "antics",
    "apparel", "ascend", "beckon", "brink", "catastrophe", "coax", "compassion", "complexion", "content",
    "courteous", "cringe", "derelict", "dignity", "distaste", "dormant", "elaborate", "endure", "enforce",
    "exertion", "expanse", "extraordinary", "foliage", "foremost", "frank", "function", "futile", "gaze",
    "glimmer", "glimpse", "grimace", "headstrong", "hesitate", "hoist", "immense", "imperceptibly",
    "indication", "inscription", "instinctive", "intent", "interior", "jar", "keepsake", "knack",
    "literacy", "lurch", "makeshift", "malicious", "massive", "meager", "melancholy", "merge", "mingle",
    "minuscule", "momentary", "nape", "nimble", "obstinate", "opt", "overwhelming", "pact", "pandemonium",
    "persuade", "phenomenal", "ponder", "quantity", "quaver", "quench", "radiant", "ravine", "recipient",
    "resentful", "satisfactory", "sensitive", "sentiment", "shudder", "sickly", "sleek", "solemn", "soothe",
    "stagger", "stern", "tantalize", "temptation", "transform", "unscrupulous", "vain", "vengeance",
    "violate", "vital", "vivid", "wistful", "yield", "zest"
]

HANGED = [P_HEAD, P_BODY, P_LEFT_ARM,
          P_RIGHT_ARM, P_LEFT_LEG, P_RIGHT_LEG]
          

def redraw():
  _.redraw()

def drawFigure(part):
  _.drawFigure(part)

def erase():
  _.clear()

def display(text):
  _.display(text)

def eraseAndDisplay(text):
  _.clearAndDisplay(text)


def warn(text):
  _.alert(text)


def ask(text):
  return _.confirm(text)
