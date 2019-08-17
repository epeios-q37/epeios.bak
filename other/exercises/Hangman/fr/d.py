# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.d import *


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def obtenirMasque(*args):
    return workshop.rfGetMask(*args)

"""
- 'parties' (tuple) contient les différentes parties du corps du pendu à
  dessiner en fonction du nombre d'erreurs '(P_TETE,P_TRONC,P_BRAS_GAUCHE…)' ;
- 'nbErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
Lorsque la dernière partie du corps est dessinée, le visage ('P_VISAGE') doit
également être dessiné.
"""
def majCorps(parties, nbErreurs):
  if nbErreurs <= len(parties):
    dessinePartieCorps(parties[nbErreurs-1])

  if nbErreurs >= len(parties):
    dessinePartieCorps(P_VISAGE)


go(globals())
