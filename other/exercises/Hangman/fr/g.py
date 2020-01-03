# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.g import *

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
Utilisation de la version alternative (pas obligatoire).
'P_VISAGE' est volontairement occulté (voir fonction suivante).
"""
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT,
#  P_VISAGE
)


"""
- 'nbErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
Lorsque la dernière partie du corps est dessinée, le visage ('P_VISAGE') doit
également être dessiné.
"""
def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  if nombreErreurs >= len(PARTIES_CORPS):
    dessinerPartieCorps(P_VISAGE)


go(globals())
