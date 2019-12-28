# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.i import *

MONTRER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


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
  def reinitialiser(self, dictionnaire,suggestion):
    self.motSecret = choisirMot(dictionnaire,suggestion)
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = VRAI

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = FAUX
    
  def traiterEtTesterPioche(self, pioche):
    if lettreEstDansMot(pioche, self.motSecret):  # Test is not mandatory
      if not lettreEstDansMot(pioche, self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nbErreurs += 1
      return FAUX



"""
Ajouter le test.
"""
def donnerMasqueEtTesterSiVictoire(mot, pioches):
  masque = ""
  victoire = VRAI

  for lettre in mot:
    if lettreEstDansMot(lettre, pioches):
      masque += lettre
    else:
      masque += "_"
      victoire = FAUX

  return masque, victoire


"""
Ajouter le test.
"""
def majCorpsEtTesterSiDefaite(parties, nbErreurs):
  if nbErreurs <= len(parties):
    dessinerPartieCorps(parties[nbErreurs-1])

  if nbErreurs >= len(parties):
    dessinerPartieCorps(P_VISAGE)
    return VRAI
  else:
    return FAUX


"""
Ajouter le test.
"""
def traiterPioche(pendu, pioche, parties):
  if pendu.traiterEtTesterPioche(pioche):
    masque, victoire = donnerMasqueEtTesterSiVictoire(pendu.motSecret,pendu.bonnesPioches)
    effacerEtAfficher(masque)
    if victoire and pendu.enCours:
      notifier("Tu as gagné ! Félicitations !")
      pendu.enCours = FAUX
  elif pendu.enCours and majCorpsEtTesterSiDefaite(parties, pendu.nbErreurs):
    notifier("\nPerdu !\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs, len(pendu.bonnesPioches),  pendu.motSecret))
    pendu.enCours = FAUX


"""
Modifier pour utiliser 'donnerMasqueEtTesterSiVictoire(…)'.
"""
def reinitialiser(pendu, dictionnaire, suggestion):
  pendu.reinitialiser(dictionnaire, suggestion)
  print(pendu.motSecret)
  effacerEtAfficher(donnerMasqueEtTesterSiVictoire(pendu.motSecret, "")[0])

  return pendu.motSecret



"""
Appelé lors d'une nouvelle connexion.
"""
def AConnexion(pendu, dictionnaire, suggestion):
  return reinitialiser(pendu, dictionnaire, suggestion)


"""
Appellé lors d'une nouvelle pioche.
NOTA: La lettre piochée sera désactivée sur le clavier.
"""
def APioche(pendu, pioche, parties):
  traiterPioche(pendu, pioche, parties)


"""
Appelé lors d'un redémarrage.
"""
def ARedemarrage(pendu, dictionnaire, suggestion):
  if pendu.enCours:
    notifier("\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs, len(pendu.bonnesPioches),  pendu.motSecret))

  return reinitialiser(pendu, dictionnaire, suggestion)

go(globals())
