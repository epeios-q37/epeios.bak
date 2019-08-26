# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.f import *

MONTRER_MOT_SECRET = VRAI

def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


class Pendu:
  def reinitialiser(self, dictionnaire,suggestion):
    self.motSecret = pickWord(dictionnaire,suggestion)
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
Pareil que pour le précédent exercice,
sauf que l'on utilise un objet.
"""
def reinitialiser(pendu,dictionnaire,suggestion):
  pendu.reinitialiser(dictionnaire,suggestion)
  print(pendu.motSecret)
  redessiner()
  effacerEtAfficher(getMask(pendu.motSecret,""))

  return pendu.motSecret
  

"""
Idem.
"""
def traiterPioche(pendu,pioche,parties):
  if pendu.traiterEtTesterPioche(pioche):
      effacerEtAfficher(getMask(pendu.motSecret, pendu.bonnesPioches))
  else:
    updateBody(parties, pendu.nbErreurs)

go(globals())
