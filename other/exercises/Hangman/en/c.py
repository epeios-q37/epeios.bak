# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *


DISCLOSE_SECRET_WORD = TRUE


"""
- 'randomWord': an random word,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise 'randomWord'.
"""
def pickWord(suggestion,randomWord):
  if suggestion:
    return suggestion
  else:
    return randomWord


go(globals())
