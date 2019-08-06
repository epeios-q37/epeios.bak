# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

# This is the prototype of the function that the student has to implement.
# The function must return true when letter is in the word, false otherwise.
def isLetterInWord(letter,word):
    if letter in word:
        return TRUE
    else:
        return FALSE

go(globals())
