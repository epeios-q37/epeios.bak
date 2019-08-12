# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.b import *

def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)

"""
Doit retourner une chaîne de caractères contenant 'mot' (qui est en minuscule) où
chaque lettre qui n'est pas contenue dans 'pioches' (également en minuscules)
est remplacée par le caractère '_'.
"""
def obtenirMasque(mot, pioches):
    masque = ""

    for lettre in mot:
        if lettreEstDansMot(lettre,pioches):
            masque += lettre
        else:
            masque += "_"

    return masque

go(globals())
