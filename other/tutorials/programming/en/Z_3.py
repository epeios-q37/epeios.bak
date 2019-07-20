# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.z_3 import *

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


class Hangman:
    def reset(self):
        self.errorsAmount = 0
        self.goodGuesses = []
        self.secretWord = ""

    def __init__(self):
        self.reset()


HANGED = [P_HEAD, P_BODY, P_LEFT_ARM,
         P_RIGHT_ARM, P_LEFT_LEG, P_RIGHT_LEG]


def getWord():
    return DICTIONARY[randint(0, len(DICTIONARY)-1)]


def updateFigure(errorsAmount):
    if (errorsAmount):
        drawFigure(HANGED[errorsAmount-1])


def updateMask(secretWord, goodGuesses):
    mask = ("_" * len(secretWord))

    for i in range(len(secretWord)):
        if secretWord[i] in goodGuesses:
            mask = mask[:i] + secretWord[i] + mask[i + 1:]

    eraseAndDisplay(mask)


def reset(hangman):
    redraw()
    hangman.reset()
    hangman.secretWord = getWord()
    print(hangman.secretWord)
    updateMask(hangman.secretWord, hangman.goodGuesses)


def connection(hangman):
    reset(hangman)


def guess(hangman, guess):
    if guess in hangman.secretWord:
        hangman.goodGuesses.append(guess)
        correct = 0
        for i in range(len(hangman.secretWord)):
            if hangman.secretWord[i] in hangman.goodGuesses:
                correct += 1
                updateMask(hangman.secretWord, hangman.goodGuesses)

        if correct == len(hangman.secretWord):
            updateMask(hangman.secretWord, hangman.goodGuesses)
            warn("You won ! Congratulations !")
            reset(hangman)
            return
    else:
        hangman.errorsAmount += 1
        updateFigure(hangman.errorsAmount)

    if hangman.errorsAmount >= len(HANGED):
        drawFigure(P_FACE)
        warn("\nYou lose !\nErrors: " + str(hangman.errorsAmount) +
             "; good guesses: " + str(len(hangman.goodGuesses)) +
             "\n\nThe secret word was: '" + hangman.secretWord + "'.")
        reset(hangman)


def restart(hangman):
    if (hangman.secretWord != ""):
        warn("Errors: " + str(hangman.errorsAmount) +
             "; good guesses: " + str(len(hangman.goodGuesses)) +
             "\n\nThe secret word was: '" + hangman.secretWord + "'.")
    reset(hangman)


go(Hangman, {
    A_CONNECTION: connection,
    A_GUESS: guess,
    A_RESTART: restart
})
