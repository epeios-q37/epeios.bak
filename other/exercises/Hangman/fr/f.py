# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.f import *


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


class Pendu:
  def reinitialiser(self, dictionnaire):
    self.motSecret = pickWord(dictionnaire)
    self.bonnesPioches = ""
    self.nbErreurs = 0

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nbErreurs = 0
    
  """
  Met à jour le nombre d'erreurs ou les bonnes pioches selon
  que 'pioche' est correct.
  Si 'pioche' est bon, retourne VRAI, sinon retourne FAUX.
  """
  def traiterEtTesterPioche(self, pioche):
    if isLetterInWord(pioche, self.motSecret):  # Test is not mandatory
      if not isLetterInWord(pioche, self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nbErreurs += 1
      return FAUX


"""
Pareil que pour le précédent objet,
sauf que l'on utilise un objet.
"""
def reinitialiser(pendu,dictionnaire):
  pendu.reinitialiser(dictionnaire)
  print(pendu.motSecret)
  redessine()
  effaceEtAffiche(getMask(pendu.motSecret,""))

"""
Idem.
"""
def traiterPioche(pendu,pioche,parties):
  if pendu.traiterEtTesterPioche(pioche):
      effaceEtAffiche(getMask(pendu.motSecret, pendu.bonnesPioches))
  else:
    updateBody(parties, pendu.nbErreurs)

go(globals())
