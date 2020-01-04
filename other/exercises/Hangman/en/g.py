# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.g import *

DISCLOSE_SECRET_WORD = TRUE

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
Use of the alternative version (not mandatory).
"P_FACE' is deliberately omitted (see next function).
"""
BODY_PARTS = (
  P_HEAD,
  P_TRUNK,
  P_LEFT_ARM,
  P_RIGHT_ARM,
  P_LEFT_LEG,
  P_RIGHT_LEG,
#  P_FACE 
)


"""
- 'errorsAmount': the amount of errors.
Draw the part of the body corresponding to the amount of errors.
When the last part of the body is drawn, the face ('P_FACE') must
also be drawn.
"""
def updateBody(errorsAmount):
  drawBodyPart(BODY_PARTS[errorsAmount-1])

  if errorsAmount >= len(BODY_PARTS):
    drawBodyPart(P_FACE)


go(globals())
