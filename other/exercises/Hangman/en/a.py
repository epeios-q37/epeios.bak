# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

"""
# Must return 'TRUE' when 'letter' is in 'word', 'FALSE' otherwise.
# 'TRUE' -> 'True' and 'FALSE' -> 'False' at whish.
"""
def isLetterInWord(letter,word):
    if letter in word:
        return TRUE
    else:
        return FALSE

go(globals())
