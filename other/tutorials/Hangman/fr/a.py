# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

"""
La focntion doit retourner 'VRAI' lordue 'lettre' est contenu dans 'mot',
'FAUX' sinon.
"""
def lettreEstDansMot(lettre, mot):
    if lettre in mot:
        return VRAI
    else:
        return FAUX


go(globals())
