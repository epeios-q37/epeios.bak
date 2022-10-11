---
menu: Étape H
title: Jeu du pendu - Étape H
weight: 8
---

# Étape H du jeu du pendu

## Objectif

Lorsque le dernier membre du corps du pendu est dessiné, le visage doit également être dessiné, pour indiquer que la partie est perdue.

## Préparation

- Dans la première ligne, remplacer le `g` par un `h`.

```python
ETAPE = "h"

m = __import__(f"workshop.fr.{ETAPE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

…

def majCorps(nombreErreurs):    
    …

go(globals())
```

## Missions

- Retirer du *tuple* `PARTIES_CORPS` la constante `P_VISAGE` ;
- modifier le code de la fonction `majCorps` pour dessiner explicitement le visage (utilisation de la constante `P_VISAGE`) lorsque la dernière partie du corps, tel que contenue dans `PARTIES_CORPS`, est dessinée.

On pourra se servir de la taille du *tuple* pour détecter quand est dessinée la dernière partie du corps.

## Pseudo-code

> Dessiner la partie du corps correspondant à `nombreErreurs`
> 
> Si `nombreErreur` supérieur au nombre d'erreurs autorisées  
> > Dessiner le visage

## Aperçu

Le même que pour l'étape précédente.

## Tests

Reprendre les tests de l'étape précédente, et vérifier qu'au lieu d'être dessiné à part, le visage est dessiné en même temps que la dernière partie du corps.