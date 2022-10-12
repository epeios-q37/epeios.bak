---
menu: Étape C
title: Jeu du pendu - Étape C
weight: 3
---

# Étape C du jeu du pendu

## Particularités

Pour la première version, aucune en particulier ; il s'agit juste d'une amélioration.

Pour la seconde version :

- tuples :
  - création,
  - récupération d'un élément par son index,
  - récupération taille ;
- import d'une fonction particulière d'un module,
- fonction `randint`.

## Solutions

Première version :

```python
def choisirMot(suggestion,motAuHasard):
  if suggestion:
    return suggestion
  else:
    return motAuHasard
```

Seconde version :

```python
from random import randint

# …

# Placer les mots de son choix.
DICTIONNAIRE = ("arbre", "maison", "chaise")


def choisirMot(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]
```

## Remarque

La seconde solution est optionnelle. On pourra y revenir plus tard, une fois les *tuple* abordés. Dans ce cas, ne pas oublier de remettre `from workshop.fr.c import *`…