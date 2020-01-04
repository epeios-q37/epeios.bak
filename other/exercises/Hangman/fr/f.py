# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.f import *

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
- 'nbErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
"""
def majCorps(nombreErreurs):
  if nombreErreurs == 1:
    partieCorps = P_TETE
  elif nombreErreurs == 2:
    partieCorps = P_TRONC
  elif nombreErreurs == 3:
    partieCorps = P_BRAS_GAUCHE
  elif nombreErreurs == 4:
    partieCorps = P_BRAS_DROIT
  elif nombreErreurs == 5:
    partieCorps = P_PIED_DROIT
  elif nombreErreurs == 6:
    partieCorps = P_PIED_GAUCHE
  elif nombreErreurs == 7:
    partieCorps = P_VISAGE

  if nombreErreurs:
    dessinerPartieCorps(partieCorps)

go(globals())
