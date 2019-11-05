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

TRUE = True
FALSE = False

P_FACE = _.P_FACE
P_HEAD = _.P_HEAD
P_TRUNK = _.P_TRUNK
P_LEFT_ARM = _.P_LEFT_ARM
P_RIGHT_ARM = _.P_RIGHT_ARM
P_LEFT_LEG = _.P_LEFT_LEG
P_RIGHT_LEG = _.P_RIGHT_LEG

DICTIONARY = (
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
)

BODY_PARTS = (
  P_HEAD,
  P_TRUNK,
  P_LEFT_ARM,
  P_RIGHT_ARM,
  P_LEFT_LEG,
  P_RIGHT_LEG
)

USER_ITEM_LABELS = {
  _.UC_HANGMAN: "Hangman",
  _.UV_SHOW_SECRET_WORD: "SHOW_SECRET_WORD",
  _.UF_GET_MASK: "getMask",
  _.UF_HANDLE_GUESS: "handleGuess",
  _.UF_IS_LETTER_IN_WORD: "isLetterInWord",
  _.UF_PICK_WORD: "pickWord",
  _.UF_RESET: "reset",
  _.UF_UPDATE_BODY: "updateBody",
  _.UF_ACONNECT: "AConnect",
  _.UF_ASUBMIT: "ASubmit",
  _.UF_ARESTART: "ARestart"
}

_.setI18n({
  _.label.Restart: "Restart",
  _.label.SecretWord: "Secret word",
  _.label.Letter: "Guess",
  _.label.Expected: "Expected",
  _.label.Obtained: "Gotten",
  # '_.label.True': syntax error!
  _.label.__dict__["True"]:  "true",
  # '_.label.False': syntax error!
  _.label.__dict__["False"]: "false",
})

_.setDictionary(DICTIONARY)
_.setBodyParts(BODY_PARTS)

_.setAppTitle("Hangman game workshop")


def redraw():
  _.redraw()


def drawBodyPart(part):
  _.drawBodyPart(part)


def erase():
  _.clear()


def display(text):
  _.display(text)


def eraseAndDisplay(text):
  _.clearAndDisplay(text)


def notify(text):
  _.alert(text)


def ask(text):
  return _.confirm(text)

