# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

# Ne pas oublier si on utilise 'DICTIONNAIRE'.
from random import randint

"""
Si absent ou mis à 'False', le champ texte
affichant le mot secret et permettant sa saisie
ne sera pas affiché (mode par défaut).
"""

MONTRER_MOT_SECRET = VRAI

# À ne définir que pour la version 4 de la fonction 'choisirMot()'.
DICTIONNAIRE = ("Arbre", "Maison", "Chaise")


"""
Version 1 :
Retourne un mot quelconque.
"""
def _choisirMot1():
  return "quelconque"



"""
Version 2 :
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot quelconque sinon.
"""
def _choisirMot1(suggestion):
  if suggestion:
    return suggestion
  else:
    return "quelconque"


"""
Pour l'exercice suivant, on pourra choisir de faire développer :
- la version 3 seule, si l'on ne souhaite pas aborder les tuples ;
- la version 3, puis la version 4 ;
- directement la version 4.
"""



"""
Version 3 :
- 'motAuHasard': un mot aléatoire ;
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, 'motAuHasard' sinon.
"""
def _choisirMot3(suggestion,motAuHasard):
  if suggestion:
    return suggestion
  else:
    return motAuHasard


"""
Version 4 :
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot au hasard sinon.
"""
def _choisirMot4(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]

VERSION = 4 # 1, 2, 3 ou 4.

choisirMot = globals()["_choisirMot" + str(VERSION)]

go(globals())
