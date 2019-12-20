# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.f import *

MONTRER_MOT_SECRET = VRAI

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
- 'parties' (tuple) contient les différentes parties du corps du pendu à
  dessiner en fonction du nombre d'erreurs '(P_TETE,P_TRONC,P_BRAS_GAUCHE…)' ;
- 'nbErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
Lorsque la dernière partie du corps est dessinée, le visage ('P_VISAGE') doit
également être dessiné.
"""
def majCorps(parties,nombreErreurs):
  if nbErreurs <= len(parties):
    dessinerPartieCorps(parties[nombreErreurs-1])

  if nombreErreurs >= len(parties):
    dessinerPartieCorps(P_VISAGE)


go(globals())
