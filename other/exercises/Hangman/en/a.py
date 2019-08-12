# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

"""
# Must return 'TRUE' when 'letter' is in 'word', 'FALSE' otherwise.
# 'TRUE' -> 'True' and 'FALSE' -> 'False' at whish.
"""
def isLetterInWord(letter,word):
    # Yes, there is a simpler way…
    for i in range(0, len(word)):
        if word[i] == letter:
            return True

    return False


go(globals())
