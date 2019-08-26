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

# Reference implementation of user functions.

import sys
sys.path.append("workshop/_")

from helpers import *

from random import randint

def rfPickWord(dictionary, suggestion):
  return suggestion if suggestion else dictionary[randrange(len(dictionary))]


def rfIsLetterInWord(letter, word):
  return letter in word


def rfGetMask(word, guesses):
  mask = ""
  
  for letter in word:
    mask += letter if letter in guesses else "_"

  return mask

def rfUpdateBody(parts,errorsAmount):
  if errorsAmount <= len(parts):
    drawBodyPart(parts[errorsAmount-1])

  if errorsAmount >= len(parts):
    drawBodyPart(P_FACE)


def rfPickWord(dictionary,suggestion=""):
  return suggestion if suggestion else dictionary[randint(0, len(dictionary)-1)]


def rfHandleGuess(hangman, guess, parts):
  if hangman.handleAndTestGuess(guess):
    eraseAndDisplay(getMask(hangman.secretWord, hangman.goodGuesses))
  else:
    updateBody(parts, hangman.errorsAmount)




