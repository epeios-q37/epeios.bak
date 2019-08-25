# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.e import *

MONTRER_MOT_SECRET = VRAI

"""
Quelques variables vont être gèrées par l'élève. Leur nom est libre.
Peut être omis, vu que 'raz(…)' va de être appelé avant utilisation
de ces variables.
"""
motSecret = ""
bonnesPioches = ""
nbErreurs = 0

"""
NOTA: les quatres fonctions suivantes ne sont pas utilisées en-dehors de ce fichier.
Bien entendu, ce seront les versions francisées écrites par les étudiants lors des
précédents exercices qui seront utilisées (cette remarque vaut également pour les
exercices suivants).
"""

def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)



"""
Reinitialise les variables et l'affichage pour une nouvelle partie.
Retourne le mot secret.
"""
def reinitialise(dictionnaire, suggestion):
  global motSecret, bonnesPioches, nbErreurs

  motSecret = pickWord(dictionnaire,suggestion)
  bonnesPioches = ""
  nbErreurs = 0
  print(motSecret, suggestion)
  redessiner()
  effacerEtAfficher(getMask(motSecret,""))

  return motSecret

  

"""
N.B. : NON 'THREAD-SAFE' !!!
- 'pioche': la lettre choisie par le joueur,
- 'mot': le mot secret,
- 'parties': (tuple) les différentes parties du corps.
Si 'pioche' est dans 'lettre', met à jour du masque,
sinon met à jour du dessin du pendu. 
"""
def traiterPioche(pioche,parties):
  global bonnesPioches, nbErreurs

  if isLetterInWord(pioche, motSecret): # Test non obligatoire.
    if not isLetterInWord(pioche,bonnesPioches):
      bonnesPioches += pioche
      effacerEtAfficher(getMask(motSecret, bonnesPioches))
  else:
    nbErreurs += 1
    updateBody(parties, nbErreurs)

go(globals())
