# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

# Ne pas oublier.
from random import randint

"""
Si absent ou mis à 'False', le champ texte
affichant le mot secret et permettant sa saisie
ne sera pas affiché (mode par défaut).
"""

MONTRER_MOT_SECRET = VRAI

DICTIONNAIRE = ("Arbre", "Maison", "Chaise")


"""
Il sera demandé aux élèves de développer les
versions 0, 1 et/ou 2 de la fonction '_choisirMot(...)',
selon que l'on veuille ou non aborder les 'tuples'
(utilisation de 'DICTIONNAIRE').
"""

"""
Retourne un mot choisi au hasard dans 'DICTIONNAIRE'.
"""
def _choisirMot0():
  return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]



"""
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot choisi au hasard dans
'DICTIONNAIRE' sinon.
"""
def _choisirMot1(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]



"""
- 'dictionnaire': tuple contenant des mots parmis
  lesquels choisir ;
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot choisi au hasard dans
'dictionnaire' sinon.
NOTA : la constante 'DICTIONNAIRE' peut naturellement être supprimée.
"""
def _choisirMot2(dictionnaire,suggestion):
  if suggestion:
    return suggestion
  else:
    return dictionnaire[randint(0, len(dictionnaire)-1)]


VERSION = 2 # 0, 1 ou 2.

choisirMot = globals()["_choisirMot" + str(VERSION)]

go(globals())
