# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.g import *

MONTRER_MOT_SECRET = VRAI

"""
Quelques variables vont être gérées par l'élève. Leur nom est libre.
Peut être omis, vu que 'raz(…)' va être appelé avant utilisation
de ces variables.
"""
motSecret = ""
bonnesPioches = ""
nbErreurs = 0

"""
NOTA: les quatres fonctions suivantes ne sont pas utilisées en-dehors de ce fichier.
"""

def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def donnerMasque(*args):
    return workshop.rfGetMask(*args)


def majCorps(*args):
  return workshop.rfUpdateBody(*args)



"""
Réinitialiser les variables et l'affichage pour une nouvelle partie et
retourner le mot secret.
"""
def reinitialiser(dictionnaire, suggestion):
  global motSecret, bonnesPioches, nbErreurs

  motSecret = choisirMot(dictionnaire,suggestion)
  bonnesPioches = ""
  nbErreurs = 0
  print(motSecret)
  redessiner()
  effacerEtAfficher(donnerMasque(motSecret,""))

  return motSecret

  

"""
N.B. : NON 'THREAD-SAFE' !!!
De multiples instances peuvent être lancées pour montrer
en quoi cela pose problème.
"""
"""
- 'pioche': la lettre choisie par le joueur,
- 'mot': le mot secret,
- 'parties': (tuple) les différentes parties du corps.
Si 'pioche' est dans 'lettre', met à jour du masque,
sinon met à jour du dessin du pendu. 
"""
def traiterPioche(pioche,parties):
  global bonnesPioches, nbErreurs

  if lettreEstDansMot(pioche, motSecret): # Test non obligatoire.
    if not lettreEstDansMot(pioche,bonnesPioches):
      bonnesPioches += pioche
      effacerEtAfficher(donnerMasque(motSecret, bonnesPioches))
  else:
    nbErreurs += 1
    majCorps(parties, nbErreurs)

go(globals())
