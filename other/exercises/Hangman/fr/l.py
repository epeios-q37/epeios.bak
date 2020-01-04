# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.l import *

DIVULGUER_MOT_SECRET = VRAI


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
Ne concerne que les méthodes 'raz(…)'
et '__init__(…)'.
"""
class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = VRAI

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nbErreurs = 0
    self.enCours = FAUX
    
  def traiterEtTesterPioche(self,pioche):
    if lettreEstDansMot(pioche,self.motSecret):
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
def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    masque,victoire = donnerMasqueEtTesterSiVictoire(pendu.motSecret,pendu.bonnesPioches)
    afficher(masque)
    if victoire and pendu.enCours:
      notifier("Tu as gagné ! Félicitations !")
      pendu.enCours = FAUX
  elif pendu.enCours and majCorpsEtTesterSiDefaite(pendu.nbErreurs):
    notifier("\nPerdu !\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs,len(pendu.bonnesPioches),pendu.motSecret))
    pendu.enCours = FAUX


"""
Modifier pour utiliser 'donnerMasqueEtTesterSiVictoire(…)'.
"""
def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasqueEtTesterSiVictoire(pendu.motSecret,"")[0])

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret(pendu.motSecret)



"""
Appelé lors d'une nouvelle connexion.
"""
def AConnexion(pendu,suggestion,motAuHasard):
  return raz(pendu,suggestion,motAuHasard)


"""
Appellé lors d'une nouvelle pioche.
NOTA: La lettre piochée sera désactivée sur le clavier.
"""
def APioche(pendu,pioche):
  traiterPioche(pendu,pioche)


"""
Appelé lorsqu'une nouvelle partie est relancée.
"""
def ARelance(pendu,suggestion,motAuHasard):
  if pendu.enCours:
    notifier("\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'.".format(pendu.nbErreurs,len(pendu.bonnesPioches),pendu.motSecret))

  raz(pendu,suggestion,motAuHasard)

go(globals())
