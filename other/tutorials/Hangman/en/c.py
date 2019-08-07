# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


go(globals())
