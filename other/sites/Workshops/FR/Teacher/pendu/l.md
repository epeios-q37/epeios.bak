---
menu: Étape L
title: Jeu du pendu - Étape L
weight: 12
---

# Étape L du jeu du pendu

## Particularités

- Renvoi de plusieurs valeurs par une fonction ;
- construction d'une chaîne de caractères avec le contenu de variables ;
- récupération des valeurs retournées par une fonction.

## Solution

```python
# Remplace 'donnerMasque(…)'
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


# Remplace 'majCorps(…)'
def majCorpsEtTesterSiDefaite(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS)
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  return nombreErreurs >= ( P_NOMBRE - 1 )


class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nombreErreurs = 0
    self.enCours = VRAI

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0
    self.enCours = FAUX

  def traiterEtTesterPioche(self,pioche):
    if lettreEstDansMot(pioche,self.motSecret):
      if not lettreEstDansMot(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


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


def majCorpsEtTesterSiDefaite(nombreErreurs):
  majCorps(nombreErreurs)

  return nombreErreurs >= ( P_NOMBRE - 1 )


def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    masque,victoire = donnerMasqueEtTesterSiVictoire(pendu.motSecret,pendu.bonnesPioches)
    afficher(masque)
    if victoire and pendu.enCours:
      notifier("Tu as gagné ! Félicitations !")
      pendu.enCours = FAUX
  elif pendu.enCours and majCorpsEtTesterSiDefaite(pendu.nombreErreurs):
    notifier("\nPerdu !\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'."
             .format(pendu.nombreErreurs,len(pendu.bonnesPioches),pendu.motSecret))
    pendu.enCours = FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasqueEtTesterSiVictoire(pendu.motSecret,"")[0])

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret(pendu.motSecret)


def AConnexion(pendu,suggestion,motAuHasard):
  return raz(pendu,suggestion,motAuHasard)


def APioche(pendu,pioche):
  traiterPioche(pendu,pioche)


def ARelance(pendu,suggestion,motAuHasard):
  if pendu.enCours:
    notifier("\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'."
             .format(pendu.nombreErreurs,len(pendu.bonnesPioches),pendu.motSecret))

  raz(pendu,suggestion,motAuHasard)
```

## Remarques

Pour la construction des chaînes de caractères passées à `notifier(…)`, on peut, dans un premier temps, utilisés l'écriture `notifier(… + … + …)`.