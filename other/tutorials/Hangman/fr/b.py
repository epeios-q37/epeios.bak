# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.b import *

def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)

"""
Doit retourner une chaîne de caractères contenant 'mot' (qui est en minuscule) où :
- les lettres de 'mot' qui sont dans 'pioches' sont mises en minuscule,
- les lettres qui ne sont PAS dans 'pioches', si 'dev' à :
  - 'VRAI', sont laissées en majuscule,
  - 'FAUX', sont remplacées par '_'.
"""
def obtenirMasque(mot, pioches, dev):
    # dev = FAUX   # Décommenter pour tester.

    masque = ""

    for lettre in mot:
        if lettre in pioches:
            masque += lettre
        elif dev:
            masque += lettre.upper()
        else:
            masque += "_"

    return masque

go(globals())
