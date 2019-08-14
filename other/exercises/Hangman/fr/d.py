# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.d import *

"""
Quelques variables vont être gèrés par l'élève. Leur nom est libre.
Peut être omis, vu que 'raz(…)' va de être appelé avant utilisation
de ces variables
"""
bonnesPioches = ""
nbErreurs = 0

"""
NOTA: les trois fonctions suivantes ne sont pas utilisées en-dehors de ce fichier.
Bien entendu, ce seront les versions francisées écrites par les étudiants lors des
précédents exercices qui seront utilisés (cette remarque vaut également pour les
exercices suivants).
"""
def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)

"""
Doit remettre à zéro les variables pour une nouvelle partie.
"""
def raz():
  global bonnesPioches, nbErreurs

  bonnesPioches = ""
  nbErreurs = 0

"""
- 'pioche': la lettre choisie par le joueur,
- 'mot': le mot secret,
- 'parties': (tuple) les différentes parties du corps.
Si 'pioche' est dans 'lettre', mise à jour du masque,
sinon mise à jour du dessin du pendu. 
"""
def traiterPioche(pioche,mot,parties):
  global bonnesPioches, nbErreurs

  if isLetterInWord(pioche, mot): # Test non obligatoire.
    if not isLetterInWord(pioche,bonnesPioches):
      bonnesPioches += pioche
      effaceEtAffiche(getMask(mot, bonnesPioches))
  else:
    nbErreurs += 1
    updateBody(parties, nbErreurs)

go(globals())
