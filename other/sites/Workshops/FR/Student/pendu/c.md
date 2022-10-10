---
menu: Étape C
title: Jeu du pendu - Étape C
weight: 3
---

# Étape C du jeu du pendu

## Objectifs

Au lieu de toujours retourner le même mot lorsque le champ de saisie est vide, on va retourner un mot au hasard fournit par le système.

## Préparation

- Dans la première ligne, remplacer le `b` par un `c` ;
- ajouter le paramètre `motAuHasard` dans la déclaration de la fonction `choisirMot`.

```python
from workshop.fr.c import *

…

def choisirMot(suggestion,motAuHasard):
    …

go(globals())
```

## Missions

Sachant que :

- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;

modifier la fonction `choisirMot` pour que :

- lorsque `suggestion` est vide, elle retourne `motAuHasard` ;
- lorsque `suggestion` n'est **pas** vide, elle retourne `suggestion`.

## Pseudo-code

> Si `suggestion` est vide  
> > Retourner `motAuHasard`  
> 
> Sinon  
> > Retourner `suggestion`

## Aperçu

Le même que pour l'exercice précédent.

## Tests

Les mêmes que pour l'exercice précédent, sauf que, lorsque le champ de saisie est vide, ce ne sera pas toujours le même mot qui sera affiché. Il se peut qu'un même mot soit affiché deux fois (ou plus) à la suite, mais c'est rare.