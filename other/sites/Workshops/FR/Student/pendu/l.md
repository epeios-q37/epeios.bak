---
menu: Étape L
title: Jeu du pendu - Étape L
weight: 12
---

# Étape L du jeu du pendu

## Objectifs

Gérer les fins de partie, afin de notifier au joueur qu'il a gagné ou perdu, ainsi qu'afficher le nombre d'erreurs et les bonnes pioches lorsque le joueur interrompt une partie en cours pour en commencer une nouvelle.

## Préparation

- Renommer `donnerMasque` en `donnerMasqueEtTesterSiVictoire` ;
- renommer `majCorps` en `majCorpsEtTesterSiDefaite` ;
- dans la méthode `Pendu.raz`, ajouter la ligne `self.enCours = VRAI` ;
- dans le constructeur `Pendu.__init__`, ajouter la ligne `self.enCours = FAUX` ;
- ajouter les déclaration suivantes avant la dernière instruction (`go(globals())`) du fichier :
  - `def AConnexion(pendu,suggestion,motAuHasard):`,
  - `def APioche(pendu,pioche):`,
  - `def ARelance(pendu,suggestion,motAuHasard):`.

```python
from workshop.fr.k import *

…


def lettreEstDansMot(lettre,mot)
  …


# Fonction 'donnerMasque' renommée en `donnerMasqueEtTesterSiVictoire`.
def donnerMasqueEtTesterSiVictoire(mot,pioches)
  …

# Fonction 'majCorps' renommée en 'majCorpsEtTesterSiDefaite'.
def majCorpsEtTesterSiDefaite(nombreErreurs):
  …
  self.enCours = VRAI


class Pendu:
  def raz(self,suggestion,motAuHasard):
    …

  def __init__(self):
    …
    self.enCours = FAUX

  def traiterEtTesterPioche(self,pioche):
    …


def raz(pendu,suggestion,motAuHasard):
  …


def traiterPioche(pendu,pioche):
  …


def AConnexion(pendu,suggestion,motAuHasard):


def APioche(pendu,pioche):


def ARelance(pendu,suggestion,motAuHasard):


go(globals())
```

## Missions

### Fonction `donnerMasqueEtTesterSiVictoire`

Sachant que :

- `mot` est une chaîne de caractères contenant le mot à deviner ;
- `pioches` étant une chaîne de caractère contenant les lettres choisies par le joueur ;

modifier le code de la fonction `donnerMasqueEtTesterSiVictoire` (précédemment nommée `donnerMasque`) pour qu'elle retourne, en plus du masque, la valeur `VRAI` si le joueur a trouvé toutes les lettres (`pioches` contient toutes les lettres contenues dans `mot`), la valeur `FAUX` dans le cas contraire.

### Fonction `majCorpsEtTesterSiDefaite`

Sachant que :

- `nombreErreurs` est le nombre d'erreurs commises par le joueur, c'est-à-dire le nombre de lettres qu'il a choisi est qui ne sont pas contenues dans le mot à deviner ;

modifier le code de la fonction `majCorpsEtTesterSiDefaite` (précédemment nommée `majCorps`) pour que, en plus de mettre à jour le dessin du corps du pendu, elle retourne la valeur `VRAI` si le dessin du pendu est complet, `FAUX` dans le cas contraire.

Rappel : la variable globale `P_NOMBRE` contient le nombre de parties dont est constitué le dessin du corps du pendu.

### Fonction `raz`

Sachant que :

- la fonction `donnerMasque` a été renommée en `donnerMasqueEtTesterSiVictoire` ;
- que cette fonction, en plus de retourner le masque, retourne également un booléen (dont on peut ignorer la valeur) ;

modifier le code de la fonction `raz` de manière à tenir compte de ces deux modifications.

### Fonction `traiterPioche`

Sachant que :

- concernant la fonction `donnerMasqueEtTesterSiVictoire` :
  
  - celle-ci remplace la fonction précédemment nommée `donnerMasque`,
  - qu'en plus de retourner le masque, elle retourne également un booléen à `VRAI` si la partie est gagnée par le joueur, à `FAUX` sinon;
- concernant la fonction `majCorpsEtTesterSiDefaite` :
  
  - celle-ci remplace la fonction précédemment nommée `majCorps`,
  - elle retourne un booléen à `VRAI` lorsque le joueur a perdu la partie, à `FAUX` sinon ;
  
  modifier le code de la fonction `traiterPioche` pour qu'elle notifie le joueur dés qu'il a gagné ou perdu. Dans le cas où le jouer a perdu la partie, il faudra également lui notifier le nombre d'erreurs qu'il a commis (nombre de lettres piochées ne se trouvant dans le mot à deviner), le nombre de bonnes pioches (nombre de lettres piochées se trouvant dans le mot à deviner), ainsi que le mot à deviner.
  
  Pour ce faire, il existe un fonction `notifier(…)` qui affiche une boite de dialogue contenant la chaîne de caractères passée en paramètre à cette fonction.
  

### Fonction `AConnexion`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;
- cette fonction est appelée à chaque fois qu'un nouveau joueur se connecte ;

écrire le code de la fonction `AConnexion` de manière à initialiser :

- l'instance `pendu` de la classe `Pendu` ;
- le masque ;
- le dessin du pendu.

On utilisera bien entendu le fonctions développées précédemment.

### Fonction `APioche`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `pioche` contient la lettre sélectionnée par le joueur ;
- cette fonction est appelée à chaque fois que le joueur sélectionne une nouvelle lettre ;

écrire le code de la fonction `APioche` de manière à mettre à jour :

- l'instance `pendu` de la classe `Pendu` ;
- le masque ;
- le dessin du pendu.

On utilisera bien entendu le fonctions développées précédemment.

### Fonction `ARelance`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;
- cette fonction est appelée à chaque fois que le joueur clique sur le bouton `Recommencer` ;

écrire le code de la fonction `ARelance` de manière à :

- notifier au joueur (on utilisera la fonction `notifier(…)` évoquée plus haut):
  - le nombre d'erreurs,
  - le nombre de bonnes pioches,
  - le mot à deviner ;
- initialiser pour une nouvelle partie :
  - l'instance `pendu` de la classe `Pendu` ;
  - le masque ;
  - le dessin du pendu.

On utilisera bien entendu le fonctions développées précédemment.

## Pseudo-code

### Fonction `donnerMasqueEtTesterSiVictoire`

> Mettre une chaîne de caractères vide dans `masque`  
> Mettre la valeur `VRAI` dans`victoire`
> 
> Pour `lettre` étant chaque lettre dans `mot`  
> > Si `lettre` est dans `pioches`  
> > > Ajouter `lettre` à `masque`  
> >
> > Sinon  
> > > Ajouter le caractère **`_`** à `masque`  
> > > Mettre `victoire` à `FAUX`
> 
> Retourner `masque` et `victoire`

### Fonction `majCorpsEtTesterSiDefaite`

Identique à celui de la fonction `majCorps` avec, en plus :

> Si `nombreErreurs` >= `P_NOMBRE`  
> > Retourner `VRAI`  
> 
> Sinon  
> > Retourner `FAUX`

### Fonction `raz`

Identique à celui de l'exercice précédent.

### Fonction `traiterPioche`

> Si `pioche` est contenu dans `motSecret`  
> > Afficher nouveau masque
> >
> > Si partie en cours et victoire  
> > > Notifier victoire  
> 
> Sinon  
> > Compléter le dessin du pendu  
> >
> > Si partie en cours et défaite  
> > > Notifier défaite et afficher informations sur la partie

### Fonction `AConnexion`

> Lancer une partie

### Fonction `APioche`

> Lancer le traitement de la lettre choisie par le joueur

### Fonction `ARelance`

> Si partie en cours  
> > Afficher informations sur la partie
> 
> Lancer une nouvelle partie

## Aperçu

Identique à celui de l'exercice précédent

## Tests

Identiques à celui de l'exercice précédents.

Vérifier que la détection d'une victoire et d'une défaite est correctement réalisée, et que les notifications sont correctes.

Vérifier qu'un click sur le bouton `Recommencer` entraîne une notification avec les informations sur la partie en cours, ainsi que le lancement d'une nouvelle partie.