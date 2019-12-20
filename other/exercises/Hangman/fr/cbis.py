# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *

from random import randint


MONTRER_MOT_SECRET = VRAI


# Placer les mots de son choix..
DICTIONNAIRE = ("arbre", "maison", "chaise")


"""
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot au hasard sinon.
"""
def choisirMot(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]


go(globals())
