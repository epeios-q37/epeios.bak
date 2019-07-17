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

import sys

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import atlastk as Atlas

from random import randint

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

HANGED_MAN = "Head Body LeftArm RightArm LeftLeg RightLeg".split()


def readAsset(path):
    return Atlas.readAsset(path, "Hangman")


class Core:
    def reset(self):
        self.errors = []
        self.correctGuesses = []
        self.secretWord = ""
        self.correct = 0

    def __init__(self):
        self.reset()


def randword():
    return DICTIONARY[randint(0, len(DICTIONARY))]


def showHanged(dom, errors):
    if (len(errors)):
    	dom.removeClass(HANGED_MAN[len(errors)-1], "hidden")


def showWord(dom, secretWord, errors, correctGuesses):
    blanks = ("_" * len(secretWord))
    output = ""
    for i in range(len(secretWord)):
        if secretWord[i] in correctGuesses:
            blanks = blanks[:i] + secretWord[i] + blanks[i + 1:]
    for letter in blanks:
        output += letter + " "

    html = Atlas.createHTML()

    html.putTagAndValue("h1", output)

    dom.setLayout("output", html)


def showBoard(dom, secretWord, errors, correctGuesses):
    showHanged(dom, errors)
    showWord(dom, secretWord, errors, correctGuesses)


def acConnect(core, dom):
    dom.setLayout("", readAsset("Main.html"))
    core.secretWord = randword()
    print(core.secretWord)
    showWord(dom, core.secretWord, core.errors, core.correctGuesses)


def acSuggest(core, dom, id):
    dom.addClass(id, "chosen")

    guess = id.lower()

    if guess in core.secretWord:
        core.correctGuesses.append(guess)
        core.correct = 0
        for i in range(len(core.secretWord)):
            if core.secretWord[i] in core.correctGuesses:
                core.correct += 1

        if core.correct == len(core.secretWord):
            showWord(dom, core.secretWord, core.errors, core.correctGuesses)
            dom.alert("You've won! Congratulations!")
            return
    else:
        core.errors += guess

    showBoard(dom, core.secretWord, core.errors, core.correctGuesses)

    if len(core.errors) >= len(HANGED_MAN):
        dom.removeClass("Face", "hidden")
        dom.alert("\nYou've run out of guesses. \nYou had " + str(len(core.errors)) +
                  " errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
                  "\nThe word was '" + core.secretWord + "'.")


def acRestart(core, dom):
	if (core.correct != len(core.secretWord)) and (len(core.errors) < len(HANGED_MAN)):
		dom.alert("You had " + str(len(core.errors)) +
				" errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
				"\nThe word was '" + core.secretWord + "'.")
	core.reset()
	dom.setLayout("", readAsset("Main.html"))
	core.secretWord = randword()
	print(core.secretWord)
	showWord(dom, core.secretWord, core.errors, core.correctGuesses)


callbacks = {
    "": acConnect,
    "Suggest": acSuggest,
	"Restart": acRestart
}

Atlas.launch(callbacks, Core, readAsset("Head.html"), "Hangman")
