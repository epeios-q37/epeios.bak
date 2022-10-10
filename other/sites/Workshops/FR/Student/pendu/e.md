---
menu: Étape E
title: Jeu du pendu - Étape E
weight: 5
---

# Étape E du jeu du pendu

## Objectifs

Affichage du masque, c'est-à-dire du mot à deviner avec dissimulation des lettres qui n'ont pas encore encore été trouvées par le joueur.

## Préparation

- Dans la première ligne, remplacer le `d` par un `e` ;
- ajouter `def donnerMasque(mot,pioches):` avant la dernière instruction du fichier.

```python
from workshop.fr.e import *

…

def lettreEstDansMot(lettre,mot):
    …

def donnerMasque(mot,pioches):

go(globals())
```

## Missions

Sachant que :

- `mot` est une chaîne de caractères contenant le mot à deviner ;
- `pioches` étant une chaîne de caractère contenant les lettres choisies par le joueur ;

écrire le code de la fonction `donnerMasque` pour qu'elle retourne `mot`, mais dont les lettres qui ne sont pas contenues dans `pioches` sont remplacées par le caractère `_`.

## Pseudo-code

> Mettre une chaîne de caractères vide dans `masque`
> 
> Pour `lettre` étant chaque lettre dans `mot`  
> > Si `lettre` est dans `pioches`  
> > > Ajouter `lettre` à `masque`  
> >
> > Sinon  
> > > Ajouter le caractère **`_`** à `masque`
> 
> Retourner `masque`

## Aperçu

![](../assets/e.png)

## Tests

Sachant que le contenu du masque est affiché dans le cadre au-dessus du clavier :

- au lancement, le masque doit être constitué d'un nombre de `_` égal au nombre de caractères du mot à deviner ;
- cliquer sur une lettre contenue dans le mot à deviner : toutes les occurrences de cette lettre contenues dans le mot à deviner doivent être dévoilées, en plus des lettres déjà dévoilées ;
- cliquer sur une lettre qui n'est **pas** contenue dans le mot à deviner : rien ne doit se passer ;
- tester des mots contenant plusieurs fois la même lettre pour vérifier que toutes les occurrences d'une même lettre soient bien dévoilées.