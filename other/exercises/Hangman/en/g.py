# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.g import *

SHOW_SECRET_WORD = TRUE

"""
For the sake of simplicity, the functions developed by the student in the
previous exercises use internal functions. Of course, the students will
use the code they wrote in the previous exercises.
"""


def pickWord(*args):
    return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


"""
- 'parts': tuple containing the different parts of the body
  to draw according to the amount of errors '(P_HEAD,P_TRUNK,P_LEFT_ARM…)'.
- 'errorsAmount': the amount of errors.
Draw the part of the body corresponding to the amount of errors.
When the last part of the body is drawn, the face ('P_FACE') must
also be drawn.
"""
def updateBody(parts,errorsAmount):
  if errorsAmount <= len(parts):
    drawBodyPart(parts[errorsAmount-1])

  if errorsAmount >= len(parts):
    drawBodyPart(P_FACE)


go(globals())
