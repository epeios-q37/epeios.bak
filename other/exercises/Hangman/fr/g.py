# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.g import *

DIVULGUER_MOT_SECRET = VRAI

"""
Á des fins de simplification, les fonctions développées par l'étudiant dans les
précédents exercices font appels à des fonctions internes. Bien entendu, les
étudiants utiliseront le code qu'ils ont écrits lors des précédents exercices.
"""


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def donnerMasque(*args):
    return workshop.rfGetMask(*args)
    

"""
Version alternative utilisant un tuple.
"""
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT,
  P_VISAGE
)


"""
- 'nbErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
"""
def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])


go(globals())
