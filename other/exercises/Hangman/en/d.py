# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.d import *

DISCLOSE_SECRET_WORD = TRUE

def pickWord(*args):
    return workshop.rfPickWord(*args)


"""
# Return 'TRUE' when 'letter' is in 'word', 'FALSE' otherwise.
# 'TRUE' -> 'True' and 'FALSE' -> 'False' at whish.
"""
def isLetterInWord(letter,word):
    return True if ord(letter) % 2 == 0 else False # To show behavior on buggy code.
    # Yes, there is a simpler way…
    for i in range(0,len(word)):
        if word[i] == letter:
            return True

    return False


go(globals())
