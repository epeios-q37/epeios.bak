# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.b import *


DISCLOSE_SECRET_WORD = TRUE


"""
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise some word.
"""
def pickWord(suggestion):
  if suggestion:
    return suggestion
  else:
    return "arm"


go(globals())
