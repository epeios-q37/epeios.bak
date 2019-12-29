# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.h import *

MONTRER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def donnerMasque(*args):
    return workshop.rfGetMask(*args)


def majCorps(*args):
  return workshop.rfUpdateBody(*args)

"""
Allons-y avec la programmation orientée objet.
Le nom de la classe doit être 'Pendu',
mais le nom des variables et des méthodes est libre.
"""
class Pendu:
  def reinitialiser(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nbErreurs = 0

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nbErreurs = 0
    
  """
  Mettre à jour le nombre d'erreurs ou les bonnes pioches selon
  que 'pioche' est correct ou non.
  Si 'pioche' est bon, retourner VRAI, sinon retourner FAUX.
  """
  def traiterEtTesterPioche(self,pioche):
    if isLetterInWord(pioche,self.motSecret):
      if not isLetterInWord(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nbErreurs += 1
      return FAUX


"""
Pareil que pour le précédent exercice,
sauf que l'on utilise un objet.
"""
def reinitialiser(pendu,suggestion,motAuHasard):
  pendu.reinitialiser(suggestion,motAuHasard)
  print(pendu.motSecret)
  redessiner()
  effacerEtAfficher(donnerMasque(pendu.motSecret,""))

  return pendu.motSecret
  

"""
Idem.
"""
def traiterPioche(pendu,pioche,parties):
  if pendu.traiterEtTesterPioche(pioche):
    effacerEtAfficher(donnerMasque(pendu.motSecret,pendu.bonnesPioches))
  else:
    majCorps(parties,pendu.nbErreurs)

go(globals())
