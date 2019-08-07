# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

"""
# Returns true 'TRUE' when letter is in the word, 'FALSE' otherwise.
# 'TRUE' -> 'True' and 'FALSE' -> 'False' at whish.
"""
def isLetterInWord(letter,word):
    if letter in word:
        return TRUE
    else:
        return FALSE

go(globals())
