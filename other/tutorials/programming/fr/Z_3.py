# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.z_3 import *

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


class Core:
    def reset(self):
        self.errors = 0
        self.correctGuesses = []
        self.secretWord = ""

    def __init__(self):
        self.reset()


PENDU = [P_TETE, P_CORPS, P_BRAS_GAUCHE, P_BRAS_DROIT, P_PIED_GAUCHE, P_PIED_DROIT]

def randword():
    return DICTIONARY[randint(0, len(DICTIONARY))]


def showHanged(errors):
    if (errors):
    	dessinePendu(PENDU[errors-1])


def showWord(secretWord, correctGuesses):
    output = ("_" * len(secretWord))
    for i in range(len(secretWord)):
        if secretWord[i] in correctGuesses:
            output = output[:i] + secretWord[i] + output[i + 1:]

    effaceEtAffiche(output)


def showBoard(secretWord, errors, correctGuesses):
    showHanged(errors)
    showWord(secretWord, correctGuesses)


def Connect(core):
    core.secretWord = randword()
    print(core.secretWord)
    showWord(core.secretWord, core.correctGuesses)


def Submit(core, id):
    guess = id.lower()

    if guess in core.secretWord:
        core.correctGuesses.append(guess)
        correct = 0
        for i in range(len(core.secretWord)):
            if core.secretWord[i] in core.correctGuesses:
                correct += 1

        if correct == len(core.secretWord):
            showWord(core.secretWord, core.correctGuesses)
            alerte("You've won! Congratulations!")
            core.reset()
            return
    else:
        core.errors += 1

    showBoard(core.secretWord, core.errors, core.correctGuesses)

    if core.errors >= len(PENDU):
        dessinePendu(P_VISAGE)
        alerte("\nYou've run out of guesses. \nYou had " + str(core.errors) +
                  " errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
                  "\n\nThe word was '" + core.secretWord + "'.")
        core.reset()


def Restart(core):
	if (core.secretWord != "" ):
		alerte("You had " + str(core.errors) +
				" errors and " + str(len(core.correctGuesses)) + " correct guesses. " +
				"\nThe word was '" + core.secretWord + "'.")
		core.reset()

	redessine()
	core.secretWord = randword()
	print(core.secretWord)
	showWord(core.secretWord, core.correctGuesses)

go(Core,{"connecter": Connect, "annoncer": Submit, "recommencer": Restart})
