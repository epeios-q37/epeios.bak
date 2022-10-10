---
menu: Étape H
title: Jeu du pendu - Étape H
weight: 8
---

# Étape H du jeu du pendu

## Particularités

Rien de particulier ; il s'agit juste d'une amélioration.

## Solution

Noter que, par rapport à la solution précédente, `PARTIES_CORPS` ne contient plus la constante `P_VISAGE`.

On pourra faire remarquer que le test sur le nombre d'erreurs autorisés, bien qu'il soit réduit d'un, reste correct, du fait qu'il s'appuie sur le nombre d'éléments contenus dans `PARTIES_CORPS`.

```python
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT
)

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS):
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  if nombreErreurs >= len(PARTIES_CORPS):
    dessinerPartieCorps(P_VISAGE)
```

## Remarque

On peut également considérer la variante (le second test est imbriqué dans le premier) :

```python
…

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS):
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

    if nombreErreurs == len(PARTIES_CORPS):
      dessinerPartieCorps(P_VISAGE)
```

Cela permet, dans certains cas, d'éviter un test inutile. Cependant, ce cas se présentant rarement, l'intérêt est limité.