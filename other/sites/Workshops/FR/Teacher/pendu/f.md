---
menu: Étape F
title: Jeu du pendu - Étape F
weight: 6
---

# Étape F du jeu du pendu

## Particularité

Instructions conditionnelles multiples (équivalent du *switch…* *case…* d'autres langages).

## Solution

```python
def majCorps(nombreErreurs):
  if nombreErreurs == 1:
    partieCorps = P_TETE
  elif nombreErreurs == 2:
    partieCorps = P_TRONC
  elif nombreErreurs == 3:
    partieCorps = P_BRAS_GAUCHE
  elif nombreErreurs == 4:
    partieCorps = P_BRAS_DROIT
  elif nombreErreurs == 5:
    partieCorps = P_PIED_DROIT
  elif nombreErreurs == 6:
    partieCorps = P_PIED_GAUCHE
  elif nombreErreurs == 7:
    partieCorps = P_VISAGE

  dessinerPartieCorps(partieCorps)
```

## Remarque

Pour éviter une erreur lorsque cette fonction est appelée avec un nombre d'erreur dépassant le maximum possible (i.e. le nombre de parties de corps disponibles), on testera ce nombre avant de dessiner. La constante `P_NOMBRE` contient le nombre de parties de corps disponibles.

Ainsi, on pourra remplacer :

```python
  dessinerPartieCorps(partieCorps)
```

par :

```python
  if nombreErreurs <= P_NOMBRE:
    dessinerPartieCorps(partieCorps)
```