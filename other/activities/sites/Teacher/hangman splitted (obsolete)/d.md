---
menu: Étape D
title: Jeu du pendu - Étape D
weight: 4
---

# Étape D du jeu du pendu

## Particularités

- Boucle `for` ;
- opérateur `in` ;
- fonction `range` ;
- index et chaîne de caractères.

## Solutions

```python
def lettreEstDansMot(lettre,mot):
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True

    return False
```

## Remarque

Une fois cette solution trouvée, on pourra proposer la solution suivante, en soulignant qu'elle est propre à *Python* (et certains autres langages).

```python
def lettreEstDansMot(lettre,mot):
  return lettre in mot
```