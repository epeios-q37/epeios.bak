---
menu: Étape I 
title: Jeu du pendu - Étape I
weight: 9
---

# Étape I du jeu du pendu

## Objectifs

Gérer la lettre sélectionnée par le joueur, de manière à mettre à jour, soit le masque, soit le dessin du pendu en fonction de la présence ou de l'absence dans le mot secret de la lettre sélectionnée (piochée) par le joueur.

## Préparation

- Dans la première ligne, remplacer le `h` par un `i` ;
- ajouter les lignes suivantes avant la dernière instruction du fichier :
  - `bonnesPioches = ""`,
  - `nombreErreurs = 0`,
  - `def raz(suggestion,motAuHasard):`,
  - `def traiterPioche(pioche,motSecret):`.

```python
from workshop.fr.i import *

…


def majCorps(nombreErreurs):    
    …


bonnesPioches = ""
nombreErreurs = 0


def raz(suggestion,motAuHasard):


def traiterPioche(pioche,motSecret):


go(globals())
```

## Missions

- `bonnesPioches` est une **chaîne de caractères** globale qui contiendra les lettres sélectionnées par le joueur et qui sont contenues dans le mot secret ;
- `nombreErreurs` est un **entier** global qui contiendra le nombre de lettres sélectionnées par le joueurs et qui ne sont **pas** contenues dans le mot secret.

### Fonction `raz`

Sachant que :

- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;
- cette fonction est appelée :
  - à chaque nouvelle instance du jeu,
  - à chaque fois que le joueur clique sur le bouton `Recommencer` pour recommencer une nouvelle partie,

écrire le code de cette fonction pour :

- réinitialiser les deux variables globales `bonnesPioches` et `nombreErreurs` ;
- stocker dans `motSecret` un nouveau mot secret, en utilisant l'une des fonctions développées précédemment ;
- afficher le masque correspondant au mot stocké dans `motSecret` :
  - en utilisant l'une des fonctions développées précédemment pour déterminer le masque,
  - en utilisant la fonction `afficher` pour afficher le masque (lui passer en paramètre le masque en question) ;
- retourner `motSecret`.

Contrairement à `bonnesPioches` et `nombreErreurs`, qui sont des variable **globales**, `motSecret` devra être une variable **locale**.

Le dessin du pendu est effacé automatiquement lorsque l'on clique sur `Recommencer`.

### Fonction `traiterPioche`

Sachant que :

- `pioche` contient la lettre sélectionnée par le joueur ;
- `motSecret` contient le mot à deviner tel que retourné par la fonction `raz(…)` ci-dessus ;

écrire le code de cette fonctions pour :

- si `pioche` est contenu dans `motSecret` :
  - ajouter `pioche` dans la la variable globale `bonnePioches`,
  - afficher le nouveau masque correspondant,
- si `pioche` n'est **pas** contenu dans `motSecret` :
  - incrémenter la variable globale `nombreErreurs`,
  - mettre à jour le dessin du pendu.

Il faudra, bien entendu, utiliser les fonctions développées dans les précedents exercices.

## Pseudo-code

### Fonction `raz`

> Initialiser variables globales
> 
> Choisir un nouveau mot à deviner
> 
> Afficher le masque correspondant à ce nouveau mot à deviner
> 
> Retourner ce nouveau mot à deviner

### Fonction `traiterPioche`

> Si `pioche` est contenu dans `motSecret`  
> > Ajouter `pioche` à `bonnePioches`  
> > Afficher le nouveau masque 
>  
> Sinon  
> > Incrémenter `nombreErreurs`  
> > Complèter le dessin du pendu

## Aperçu

Identique à celui de l'exercice précédent.

## Tests

Identiques à ceux de l'exercice précédent.