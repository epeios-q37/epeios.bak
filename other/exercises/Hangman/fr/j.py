# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.j import *

DIVULGUER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
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
  def raz(self):
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.raz()
    
  """
  Mettre à jour le nombre d'erreurs ou les bonnes pioches selon
  que 'pioche' est correct ou non.
  Si 'pioche' est bon, retourner VRAI, sinon retourner FAUX.
  """
  def traiterEtTesterPioche(self,pioche,motSecret):
    if lettreEstDansMot(pioche,motSecret):
      if not lettreEstDansMot(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


"""
Pareil que pour le précédent exercice,
sauf que l'on utilise un objet.
"""
def raz(pendu,suggestion,motAuHasard):
  pendu.raz()
  motSecret = choisirMot(suggestion,motAuHasard)
  print(motSecret)
  afficher(donnerMasque(motSecret,""))

  return motSecret
  

"""
Idem.
"""
def traiterPioche(pendu,pioche,motSecret):
  if pendu.traiterEtTesterPioche(pioche,motSecret):
    afficher(donnerMasque(motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)

go(globals())
