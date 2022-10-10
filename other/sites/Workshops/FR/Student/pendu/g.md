---
menu: Étape G
title: Jeu du pendu - Étape G 
weight: 7
---

# Étape G du jeu du pendu


## Objectif

Simplification de la fonction `majCorps`, grâce à l'utilisation d'un *tuple*.

## Préparation

- Dans la première ligne, remplacer le `f` par un `g`.

```python
from workshop.fr.g import *

…

def majCorps(nombreErreurs):    
    …

go(globals())
```

## Missions

- Créer un *tuple* appelé `PARTIES_CORPS` contenant les constantes `P_…` listées dans l'exercice précédent, dans l'ordre dans lequel les parties du corps correspondantes doivent être dessinées ;
- modifier la fonction `majCorps` pour qu'elle réalise la même tâche que dans l'exercice précédent, mais en utilisant le *tuple* ci-dessus.

## Pseudo-code

> Affecter à `PARTIES_CORPS` un *tuple* contenant les constantes `P_…`
> 
> Dessiner la partie du corps stockée dans `PARTIES_CORPS` correspondant à `nombreErreurs`

## Aperçu

Le même que pour l'exercice précédent.

## Test

Les mêmes que pour l'exercice précédent.