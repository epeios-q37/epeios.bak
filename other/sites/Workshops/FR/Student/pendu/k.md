---
menu: Étape K
title: Jeu du pendu - Étape K
weight: 11
---

# Étape K du jeu du pendu

## Objectifs

Prendre en charge la gestion du mot à deviner ainsi que sa (non-)divulgation en fonction de la valeur de la variable globale `DIVULGUER_MOT_SECRET`.

## Préparation

- Dans la première ligne, remplacer le `j` par un `k` ;
- dans la méthode `Pendu.raz`, ajouter les paramètres `suggestion` et `motAuHasard` ;
- dans le constructeur `Pendu.__init__(self)`, ajouter la ligne `self.motSecret = ""` ;
- retirer le paramètre `motSecret` de la fonction `traiterPioche`.

```python
from workshop.fr.k import *

…


def majCorps(*args):
  …


class Pendu:
  def raz(self,suggestion,motAuHasard):
    …

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def traiterEtTesterPioche(self,pioche):
    …


def raz(pendu,suggestion,motAuHasard):
  …


def traiterPioche(pendu,pioche):
  …


go(globals())
```

## Missions

### Méthode `Pendu.raz`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;

modifier la méthode `Pendu.raz`, en s'inspirant de la **fonction** `raz`, pour stocker, dans la variable membre `Pendu.motSecret`, un nouveau mot à deviner.

### Fonction `raz`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;

adapter la **fonction** `raz` pour prendre en compte les modifications de la **méthode** `Pendu.raz`, la gestion du mot à deviner ayant été déléguée à cette méthode de la classe `Pendu`.

Sachant qu'il existe une fonction `divulguerMotSecret(…)` qui affiche, afin de facilité les tests, la chaîne de caractère passée en paramètre, modifier la fonction `raz` de manière à afficher ou non le mot secret en fonction de la valeur de la variable globale `DIVULGUER_MOT_SECRET`.

Sachant que l'on gère maintenant le mao à deviner `motSecret`, il n'est **plus** utile :

- d'utiliser une variable locale `motSecret` ;
- de retourner le mot à deviner.

### Fonction `traiterPioche`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `pioche` contient la lettre sélectionnée par le joueur ;
- le mot à deviner :
  - n'est plus passé en paramètre à la fonction `traiterPioche` ;
  - est stocké dans la variable membre de la classe pendu `Pendu.motSecret` ;

modifier le code de la fonction `traiterPioche` en conséquence.

## Pseudo-code

### Méthode `Pendu.raz`

> Affecter à `pendu.motSecret` un nouveau mot à deviner
> 
> Réinitialiser les variables membres `Pendu.bonnesPioches` et `Pendu.nombreErreurs`

### Fonction `raz`

> Réinitialiser `pendu`
> 
> Afficher le masque correspondant au nouveau mot à deviner stocké dans l'instance de la classe `Pendu`
> 
> Si `DIVULGUER_MOT_SECRET` à `VRAI`  
> > Divulguer le mot secret

### Fonction `traiterPioche`

> Si `pendu.traiterEtTesterPioche(pioche)` retourne `VRAI`  
> > Afficher le nouveau masque  
> 
> Sinon  
> > Complèter le dessin du pendu

## Aperçu

Identique à l'exercice précédent, sauf dans le cas où `DIVULGUER_MOT_SECRET` est à `FAUX`, auquel cas le champ texte à gauche du bouton `Recommencer` ne doit pas s'afficher.

## Tests

Identiques à ceux de l'exercice précedent, en vérifiant que le champ texte à gauche du bouton `Recommencer` n'est pas affiché lorsque `DIVULGUER_MOT_SECRET` est à `FAUX`.