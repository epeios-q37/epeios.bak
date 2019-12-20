# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.b import *


MONTRER_MOT_SECRET = VRAI


"""
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot quelconque sinon.
"""
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "bras"


go(globals())
