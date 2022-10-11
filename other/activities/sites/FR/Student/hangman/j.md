---
menu: Étape J
title: Jeu du pendu - Étape J
weight: 10
---

# Étape J du jeu du pendu

## Objectifs

Comme vous avez pu le constater, la manière dont est codé l'étape précédente ne permet pas de jouer à ce jeu à plusieurs simultanément. L'objectif de cette étape est de remédier à cela, grâce à la programmation objet.

## Préparation

- Dans la première ligne, remplacer le `i` par un `j` ;
  
- supprimer la déclaration des variables `bonnesPioches` et `nombreErreurs` ;
  
- là où il y avait la déclaration de ces variables, placer le code suivant :
  
  ```python
  class Pendu:
  def raz(self):
    self.bonnesPioches = ""
    self.nombreErreurs = 0
  
  def __init__(self):
    self.raz()
  
  def traiterEtTesterPioche(self,pioche,motSecret):
  ```
  
- dans les fonctions `raz(…)` et `traiterPioches(…)`
  
  - enlever la ligne `global bonnesPioches, nombreErreurs`,
  - ajouter le paramètre `pendu` en première position dans leurs déclarations respectives.

```python
ETAPE = "j"

m = __import__(f"workshop.fr.{ETAPE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

…


def majCorps(nombreErreurs):    
    …

# Les déclarations des variables 'bonnesPioches' et 'nombreErreurs'
# qui étaient positionnées ici doivent être supprimées.

class Pendu:
  def raz(self):
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.raz()

  def traiterEtTesterPioche(self,pioche,motSecret):


def raz(pendu,suggestion,motAuHasard):
# La ligne 'global bonnesPioches, nombreErreurs' présente ici doit être supprimée.
# Le reste du code doit être conservé.
    …


def traiterPioche(pendu,pioche,motSecret):
# La ligne 'global bonnesPioches, nombreErreurs' présente ici doit être supprimée.
# Le reste du code doit être conservé.
    …

go(globals())    
```

## Missions

Au lieu d'être globales à tout le programme, les variables `bonnesPioches` et `nombreErreurs` sont maintenant rattachées à une classe appelée `Pendu`. Une instance de cette classe est crée pour chaque joueur, et est accessible via le paramètre `pendu` passé aux fonctions `raz(…)` et `traiterPioche(…)`.

Pour accèder aux membre de cette instance, on utilisera l'écriture `pendu.<variable ou méthode>`. Ainsi, `pendu.nombreErreurs` donne accès à la variable `nombreErreurs` de la classe `Pendu`, et `pendu.raz()` appellera la méthode `raz` de cette même classe.

Dans le cas de méthodes, le premier paramètre, nommé par convention `self`, est l'instance à partir de laquelle cette fonction a été appelée. Ainsi, en appelant `pendu.raz()`, le paramètre `self` de la méthode `raz` référence la même instance de la classe `Pendu` que la variable `pendu`.

### Méthode `Pendu.traiterEtTesterPioche`

Sachant que :

- `self` est l'instance de la classe `Pendu` ;
- `pioche` contient la lettre sélectionnée par le joueur ;
- `motSecret` étant le mot à deviner ;

écrire le code de la méthode `Pendu.traiterEtTesterPioche` pour qu'elle :

- remplisse les même fonctionnalités que la fonction `traiterPioche(…)` de l'étape précédente, mais en utilisant le paramètre `self` pour accèder aux variables `bonnesPioches` et `nombreErreurs` de la classe `Pendu` (`self.bonnesPioches` et `self.nombreErreurs`) ;
- retourne :
  - `VRAI` lorsque `pioche` est présent dans `motSecret`,
  - `FAUX` lorsque `pioche` n'est **pas** présent dans `motSecret`.

### Fonction `raz`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `suggestion` est le contenu du champ de saisie ;
- `motAuHasard` est un mot choisi au hasard ;
- cette fonction est appelée dans les mêmes conditions que la fonction homonyme de l'étape précédente ;

écrire le code de la fonction `raz` de manière à ce qu'elle remplisse les même fonctionnalités que dans l'étape précédente, en utilisant la méthode `Pendu.raz` pour réinitialiser les variables.

### Fonction `traiterPioche`

Sachant que :

- `pendu` est une instance de la classe `Pendu` ;
- `pioche` contient la lettre sélectionnée par le joueur ;
- `motSecret` contient le mot à deviner tel que retourné par la fonction `raz(…)` ci-dessus ;

écrire le code de la fonction `traiterPioche` de manière à ce qu'elle remplisse les même fonctionnalités que dans l'étape précédente, en utilisant la méthode `Pendu.traiterEtTesterPioche` développée précédemment.

## Pseudo-code

### Méthode `Pendu.traiterEtTesterPioche`

> Si `pioche` est contenu dans `motSecret`  
> > Ajouter `pioche` à `bonnePioches`  
> > Retourner `VRAI` 
>  
> Sinon  
> > Incrémenter `nombreErreurs`  
> > Retourner `FAUX`

### Fonction `raz`

> Réinitialiser `pendu`
> 
> Choisir un nouveau mot à deviner
> 
> Afficher le masque correspondant à ce nouveau mot à deviner
> 
> Retourner ce nouveau mot à deviner

### Fonction `traiterPioche`

> Si `pendu.traiterEtTesterPioche(pioche,motSecret)` retourne `VRAI`  
> > Afficher le nouveau masque
>  
> Sinon  
> > Complèter le dessin du pendu

### Aperçu

Identique à celui de l'étape précédente.

### Tests

Identiques à ceux de l'étape précédente, en vérifiant que cela fonctionne également avec plusieurs joueurs simultanés.