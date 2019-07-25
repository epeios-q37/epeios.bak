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
        self.errors = 0
        self.correctGuesses = []
        self.secretWord = ""

    def __init__(self):
        self.reset()


def randword():
    return DICTIONARY[randint(0, len(DICTIONARY))]


def showHanged(dom, errors):
    if (errors):
    	dom.removeClass(HANGED_MAN[errors-1], "hidden")


def showWord(dom, secretWord, correctGuesses):
    output = ("_" * len(secretWord))
    for i in range(len(secretWord)):
        if secretWord[i] in correctGuesses:
            output = output[:i] + secretWord[i] + output[i + 1:]

    html = Atlas.createHTML()
    html.putTagAndValue("h1", output)
    dom.setLayout("output", html)


def showBoard(dom, secretWord, errors, correctGuesses):
    showHanged(dom, errors)
    showWord(dom, secretWord, correctGuesses)


def acConnect(core, dom):
    dom.setLayout("", readAsset("Main.html"))
    core.secretWord = randword()
    print(core.secretWord)
    showWord(dom, core.secretWord, core.correctGuesses)


def acSubmit(core, dom, id):
    dom.addClass(id, "chosen")

    guess = id.lower()

    if guess in core.secretWord:
        core.correctGuesses.append(guess)
        correct = 0
        for i in range(len(core.secretWord)):
            if core.secretWord[i] in core.correctGuesses:
                correct += 1

        if correct == len(core.secretWord):
            showWord(dom, core.secretWord, core.correctGuesses)
            dom.alert("You've won! Congratulations!")
            core.reset()
            return
    else:
        core.errors += 1

    showBoard(dom, core.secretWord, core.errors, core.correctGuesses)

    if core.errors >= len(HANGED_MAN):
        dom.removeClass("Face", "hidden")
        dom.alert("\nYou've run out of guesses. \nYou had " + str(core.errors) +
                  " errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
                  "\n\nThe word was '" + core.secretWord + "'.")
        core.reset()


def acRestart(core, dom):
	if (core.secretWord != "" ):
		dom.alert("You had " + str(core.errors) +
				" errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
				"\nThe word was '" + core.secretWord + "'.")
		core.reset()

	dom.setLayout("", readAsset("Main.html"))
	core.secretWord = randword()
	print(core.secretWord)
	showWord(dom, core.secretWord, core.correctGuesses)


callbacks = {
    "": acConnect,
    "Submit": acSubmit,
	"Restart": acRestart
}

Atlas.launch(callbacks, Core, readAsset("Head.html"), "Hangman")