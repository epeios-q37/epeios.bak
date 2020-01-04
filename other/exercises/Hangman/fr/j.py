# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.j import *

MONTRER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def majCorps(*args):
  return workshop.rfUpdateBody(*args)


"""
En premier lieu, ne pas traiter la variable membre
'enCours' de la class 'Pendu'.
"""

"""
Ajouter le traitement de la variable 'enCours'.
Ne concerne que les méthodes 'reinitialiser(…)'
et '__init__(…)'.
"""
class Pendu:
  def reinitialiser(self,suggestion,motAuHasard):
#    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = VRAI

    return choisirMot(suggestion,motAuHasard)

  def __init__(self):
 #   self.motSecret = ""
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = FAUX
    
  def traiterEtTesterPioche(self,pioche,motSecret):
    if lettreEstDansMot(pioche,motSecret):
      if not lettreEstDansMot(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nbErreurs += 1
      return FAUX



"""
Ajouter le test.
"""
def donnerMasqueEtTesterSiVictoire(mot,pioches):
  masque = ""
  victoire = VRAI

  for lettre in mot:
    if lettreEstDansMot(lettre,pioches):
      masque += lettre
    else:
      masque += "_"
      victoire = FAUX

  return masque,victoire


"""
Ajouter le test.
"""
def majCorpsEtTesterSiDefaite(nbErreurs):
  majCorps(nbErreurs)

  return nbErreurs >= ( P_NOMBRE - 1 )


"""
Ajouter le test.
"""
def traiterPioche(pendu,pioche,motSecret):
  if pendu.traiterEtTesterPioche(pioche,motSecret):
    masque,victoire = donnerMasqueEtTesterSiVictoire(motSecret,pendu.bonnesPioches)
    effacerEtAfficher(masque)
    if victoire and pendu.enCours:
      notifier("Tu as gagné ! Félicitations !")
      pendu.enCours = FAUX
  elif pendu.enCours and majCorpsEtTesterSiDefaite(pendu.nbErreurs):
    notifier("\nPerdu !\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs,len(pendu.bonnesPioches),motSecret))
    pendu.enCours = FAUX


"""
Modifier pour utiliser 'donnerMasqueEtTesterSiVictoire(…)'.
"""
def reinitialiser(pendu,suggestion,motAuHasard):
  motSecret = pendu.reinitialiser(suggestion,motAuHasard)
  print(motSecret)
  effacerEtAfficher(donnerMasqueEtTesterSiVictoire(motSecret,"")[0])

  return motSecret



"""
Appelé lors d'une nouvelle connexion.
"""
def AConnexion(pendu,suggestion,motAuHasard):
  return reinitialiser(pendu,suggestion,motAuHasard)


"""
Appellé lors d'une nouvelle pioche.
NOTA: La lettre piochée sera désactivée sur le clavier.
"""
def APioche(pendu,pioche,motSecret):
  traiterPioche(pendu,pioche,motSecret)


"""
Appelé lors d'un redémarrage.
"""
def ARedemarrage(pendu,suggestion,motAuHasard,motSecret):
  if pendu.enCours:
    notifier("\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs,len(pendu.bonnesPioches),motSecret))

  return reinitialiser(pendu,suggestion,motAuHasard)

go(globals())
