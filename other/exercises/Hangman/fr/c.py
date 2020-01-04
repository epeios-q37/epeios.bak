# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *


DIVULGUER_MOT_SECRET = VRAI


"""
- 'motAuHasard': un mot aléatoire ;
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, 'motAuHasard' sinon.
"""
def choisirMot(suggestion,motAuHasard):
  if suggestion:
    return suggestion
  else:
    return motAuHasard


go(globals())
