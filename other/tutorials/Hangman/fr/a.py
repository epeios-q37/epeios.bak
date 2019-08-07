# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

"""
La fonction doit retourner 'VRAI' lordue 'lettre' est contenu dans 'mot',
'FAUX' sinon.
'VRAI' -> 'True' et 'FAUX' -> 'False' si souhaité.
"""
def lettreEstDansMot(lettre, mot):
    if lettre in mot:
        return VRAI
    else:
        return FAUX


go(globals())
