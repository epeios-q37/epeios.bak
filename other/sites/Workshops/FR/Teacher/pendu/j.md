---
menu: Étape J
title: Jeu du pendu - Étape J
weight: 10
---

# Étape J du jeu du pendu

## Particularités

Programmation orientée objet :

- Classe ;
- fonction membre (méthode) ;
- variable membre ;
- constructeur ;
- utilisation de la *POO* pour gérer le *thread-safety*.

## Solution

```python
class Pendu:
  def raz(self):
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.raz()

  def traiterEtTesterPioche(self,pioche,motSecret):
    if isLetterInWord(pioche,motSecret):
      if not isLetterInWord(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz()
  motSecret = choisirMot(suggestion,motAuHasard)
  print(motSecret)
  afficher(donnerMasque(motSecret,""))

  return motSecret


def traiterPioche(pendu,pioche,motSecret):
  if pendu.traiterEtTesterPioche(pioche,motSecret):
    afficher(donnerMasque(motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)
```

## Remarques

On peut signaler que `monInstance.maFonction(…)` est équivalent à `maFonction(monInstance,…)`.