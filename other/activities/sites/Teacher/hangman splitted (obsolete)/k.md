---
menu: Étape K
title: Jeu du pendu - Étape K
weight: 11
---

# Étape K du jeu du pendu

## Particularités

- Utilisation d'une variable globale comme constante ;
- type booléen ;
- manipulation d'une instance de classe.

## Solution

```python
class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def traiterEtTesterPioche(self,pioche):
    if isLetterInWord(pioche,self.motSecret):
      if not isLetterInWord(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasque(pendu.motSecret,""))

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret( pendu.motSecret )


def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    afficher(donnerMasque(pendu.motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)
```

## Remarques

Néant