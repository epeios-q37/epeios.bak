# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def obtenirMasque(*args):
    return workshop.rfGetMask(*args)


go(globals())
