# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


"""
'parts' is a tuple containing the different parts of the body
to draw according to the amount of errors '(P_HEAD,P_TRUNK,P_LEFT_ARM…)'.
'errorsAmount' is the amount of errors.
This function must draw the part of the body corresponding
to the amount of errors.
When the last part of the body is drawn, the face ('P_FACE') must
also be drawn.
"""
def updateBody(parts,errorsAmount):
  if errorsAmount <= len(parts):
    drawBodyPart(parts[errorsAmount-1])

  if errorsAmount >= len(parts):
    drawBodyPart(P_FACE)


go(globals())
