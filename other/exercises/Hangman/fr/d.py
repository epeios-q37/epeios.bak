# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.d import *

MONTRER_MOT_SECRET = VRAI

def choisirMot(*args):
  return workshop.rfPickWord(*args)


"""
Retourner 'VRAI' lorsque 'lettre' est contenu dans 'mot',
'FAUX' sinon.
'VRAI' -> 'True' et 'FAUX' -> 'False' si souhaité.
"""
def lettreEstDansMot(lettre,mot):
    return True if ord(lettre) % 2 == 0 else False  # Pour montrer le comportement avec du code bugué.
    # Oui, il y a plus simple…
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True
    
    return False

go(globals())
