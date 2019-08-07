# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.b import *

def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)

"""
Must return a string containing 'word' (which is lowercased) where:
- letters of 'word' which are in 'guesses' are lowered,
- letters which are NOT in 'guesses', if 'dev' at:
  - 'TRUE', are kept uppercased,
  - 'FALSE', are replaced by '_'.
"""
def getMask(word, guesses, dev):
    # dev = FALSE   # Uncomment for testing 

    mask = ""

    for letter in word:
        if letter in guesses:
            mask += letter
        elif dev:
            mask += letter.upper()
        else:
            mask += "_"

    return mask

go(globals())
