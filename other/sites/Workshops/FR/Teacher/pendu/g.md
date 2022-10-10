---
menu: Étape G
title: Jeu du pendu - Étape G
weight: 7
---

# Étape G du jeu du pendu

## Particularités

- Création d'un *tuple* ;
- récupération d'un membre d'un *tuple* par son index ;
- récupération du nombre d'éléments d'un *tuple*.

## Solution

```python
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT,
  P_VISAGE
)

def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```

## Remarques

Dans un second temps, pour éviter une erreur lors du dépassement du nombre d'erreurs autorisées, on remplacera :

```python
def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```

par :

```python
def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS)
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```

Noter que, pour cet exercice, et les suivants, on peut stocker le résultat de `len(PARTIES_CORPS)` dans une constante juste aprés la définition de `PARTIES_CORPS`, et utiliser cette constante en lieu et place des appels à `len(PARTIES_CORPS)`.