# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def obtenirMasque(*args):
    return workshop.rfGetMask(*args)

"""
'parties' est un tuple contenant les différentes parties du corps
à dessiner en fonction du nombre d'erreurs '(P_TETE,P_TRONC,P_BRAS_GAUCHE…)'.
'nbErreurs' est le nombre d'erreurs.
Cette fonction doit dessiner la partie du corps correspondant
au nombre d'erreurs.
Lorsque la dernière partie du corps est dessinée, le visage ('P_VISAGE') doit
également être dessiné.
"""
def majCorps(parties, nbErreurs):
  if nbErreurs <= len(parties):
    dessinePartieCorps(parties[nbErreurs-1])

  if nbErreurs >= len(parties):
    dessinePartieCorps(P_VISAGE)


go(globals())
