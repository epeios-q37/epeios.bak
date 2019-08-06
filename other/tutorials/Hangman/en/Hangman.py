# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.Hangman import *

from random import randint


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
            warn("You won! Congratulations!")
            reset(hangman)
            return
    else:
        hangman.errorsAmount += 1
        updateFigure(hangman.errorsAmount)

    if hangman.errorsAmount >= len(HANGED):
        drawFigure(P_FACE)
        warn("\nYou lose!\nErrors: " + str(hangman.errorsAmount) +
             "; good guesses: " + str(len(hangman.goodGuesses)) +
             "\n\nThe secret word was: '" + hangman.secretWord + "'.")
        reset(hangman)


def restart(hangman):
    if (hangman.secretWord != ""):
        warn("Errors: " + str(hangman.errorsAmount) +
             "; good guesses: " + str(len(hangman.goodGuesses)) +
             "\n\nThe secret word was: '" + hangman.secretWord + "'.")
    reset(hangman)


go({
    A_CONNECTION: connection,
    A_GUESS: guess,
    A_RESTART: restart
})
