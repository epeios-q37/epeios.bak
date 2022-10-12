---
menu: Étape B
title: Jeu du pendu - Étape B
weight: 2
---

# Étape B du jeu du pendu

## Particularités

- Paramètre de fonction ;
- instruction conditionnelle ;
- taille d'une chaîne de caractères ;
- booléen :
  - test,
  - transtypage ;
- opérateur de comparaison ;
- instruction conditionnelles.

## Solution

```python
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "arbre"
```

## Remarque

Une fois que la solution ci-dessus trouvée, on pourra proposer de remplacer `if len(suggestion) != 0:` par `if len(suggestion):`, puis par `if suggestion:`, en soulignant que c'est propre à *Python* (et à certains autres langages).