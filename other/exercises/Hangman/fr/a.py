# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

"""
Retourner 'VRAI' lorsque 'lettre' est contenu dans 'mot',
'FAUX' sinon.
'VRAI' -> 'True' et 'FAUX' -> 'False' si souhaité.
"""
def lettreEstDansMot(lettre, mot):
    # Oui, il y a plus simple…
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True
    
    return False

go(globals())
