# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.d import *

# Ne pas oublier.
from random import randint


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def obtenirMasque(*args):
    return workshop.rfGetMask(*args)


def majCorps(*args):
  return workshop.rfUpdateBody(*args)


"""
- 'dictionnaire': tuple contenant ds mots parmis
  mesqueles choisir ;
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot choisi au hasard dans
'dictionnaire' sinon.
"""
def choisirMot(dictionnaire,suggestion):
  if suggestion:
    return suggestion
  else:
    return dictionnaire[randint(0, len(dictionnaire)-1)]


go(globals())
