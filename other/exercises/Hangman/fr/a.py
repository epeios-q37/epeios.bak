# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *


"""
Si absent ou mis à 'False', le champ texte
affichant le mot secret et permettant sa saisie
ne sera pas affiché (mode par défaut).
"""
DIVULGUER_MOT_SECRET = VRAI


"""
Retourne un mot quelconque.
"""
def choisirMot():
  return "pied"


go(globals())
