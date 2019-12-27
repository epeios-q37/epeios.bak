# Exercices de programmation du jeu du pendu ; documentation professeur

## Accès rapide

Cette section permet de retrouver rapidement les différents liens indiqués dans le reste du document.

- Documents professeur :
  - ce document : <https://q37.info/s/mmdnch9t>,
  - le dépôt *GitHub* contenant les solutions aux exercices : <https://github.com/epeios-q37/hangman-fr-teacher>,
  - pour tester ces solutions en ligne : <https://repl.it/github/epeios-q37/hangman-fr-teacher> ;
- documents élèves :
  - le document contenant les instructions pour les élèves : <https://q37.info/s/zmg4m3fx>,
  - le dépôt *GitHub* contenant les fichiers destinés aux élèves : <https://github.com/epeios-q37/hangman-fr>,
  - pour faire travailler les élèves sur ces exercices dans un environnement de développement en ligne : <https://repl.it/github/epeios-q37/hangman-fr>.


## Introduction

De nos jour, la plupart des jeunes possèdent un smartphone. Il est donc plus facile de les intéresser à la programmation avec des exercices qui s'utilisent comme les applications de leurs smartphones.

C'est pourquoi les exercices présentés ici ont une véritable interface graphique, au lieu de l'habituelle et austère interface texte. De plus, avec chaque exercice, un [code QR](https://fr.wikipedia.org/wiki/Code_QR) permettant d'y avoir accès à partir d'un smartphone est affiché. Ainsi, ils verront que l'usage de leurs  smartphones n'est pas limité à des applications comme *Facebook*, *YouTube*, *Twitter*….

En outre, ils peuvent partager cet accès avec leurs proches, directement sur leurs smartphones (ou n'importe quel dispositif équipé d'un navigateur web moderne connecté à internet) respectifs. Ils recueilleront ainsi plus facilement des encouragements, les incitant à progresser.

Vous trouverez plus de détails sur ce nouveau type d'exercices à cette l'adresse <https://q37.info/s/knf9hdwp>.

Ces exercices aboutissent à la programmation, en *Python*, du [jeu du pendu](https://fr.wikipedia.org/wiki/Le_Pendu_(jeu)) avec une interface graphique, sans avoir à manipuler directement cette interface. Chaque exercice s'appuie sur le précédent et l'ensemble reproduit les différentes étapes menant à l'élaboration d'un tel jeu.

Contrairement aux autres exercices de programmation, chaque exercice ne se concentre pas sur un concept, mais sur une fonctionnalité. L'assimilation des concepts informatiques est ainsi facilitée, car n'étant pas une fin en soi, mais seulement un moyen de parvenir au but, à savoir la programmation du jeu.

Ces exercices portent sur la programmation en générale, et non pas sur la programmation en *Python*. C'est pour cela que les solutions générales seront données avant les solutions propres à *Python*.

## Utilisation

Le dépôt https://github.com/epeios-q37/hangman-fr est destiné aux élèves. https://github.com/epeios-q37/hangman-fr-teacher reprend le contenu de ce dépôt, en y ajoutant des informations utiles aux professeurs, notamment les solutions des exercices. De ce fait, ce dernier dépôt est réservé aux professeurs.

Pour distribuer ces exercices aux élèves, vous pouvez directement utiliser le dépôt https://github.com/epeios-q37/hangman-fr ou, si vous êtes familiarisés avec *GitHub*, en utiliser les outils mis à disposition pour créer votre propre dépôt. 

Ls exercices ne sont pas destinés à être enchaînés directement, mais traités au fur et à mesure que les différentes notions informatiques nécessaires auront été abordées par ailleurs. Vous pouvez tester ces exercices en allant à cette adresse : https://repl.it/github/epeios-q37/hangman-fr-teacher. Les fichiers contenus dans le répertoire `Student` sont ceux mis à disposition des étudiants.

Il existe deux façon pour les élèves de travailler sur ces exercices. Soit en utilisant un ordinateur avec *Python* d'installé (utilisation en local), soit dans un navigateur web (utilisation en ligne). 

### Utilisation en local

Pour une utilisation en local, sur un ordinateur équipé de *Python*, il suffit aux élèves de télécharger et désarchiver le fichier suivant : https://github.com/epeios-q37/hangman-fr/…. Alternativement, s'ils possèdent les connaissances adéquates, ils peuvent aussi faire un `git clone https://github.com/epeios-q37/hangman-fr`.

Les exercices consistent à écrire du code dans le fichier `pendu.py` situé à la racine du dépôt. Pour le lancement, il suffira d'exécuter la commande `python3 pendu.py` dans une console, à partir de la racine du dépôt. S'ouvrira alors un navigateur web leur donnant accès à leur exercice. Le programme sera arrêté avec un *CTRL-C*.

Ce dépôt contient, à la racine, un fichier `LISEZMOI.html` qui leur donnent les instructions pour chaque exercice.

### Utilisation en ligne

Pour une utilisation en ligne, en utilisant un navigateur web, sans rien avoir à installer, il suffit de suivre le lien suivant : https://repl.it/github/epeios-q37/hangman-fr. Cela ouvre ce dépôt dans *Repl.it*, un environnement de développement en ligne.

Le fichier `LISEZMOI.html` leur donnant les instructions pour chaque exercice se situe à l'adresse  https://q37.info/s/zmg4m3fx. Il faut l'ouvrir via cette adresse, car *GitHub*, comme *Repl.it*, n'en afficheront que le code source.

En plus de ce lien, il faudra leur faire ouvrir, dans une fenêtre ou un autre onglet le lien suivant qui leur donnera accès au fichier `LISEZMOI.html` leur donnant les instructions pour chaque exercice : https://q37.info/s/zmg4m3fx.

Comme ci-dessus, l'exercice consistera à écrire du code dans le fichier `pendu.py`. Pour lancer l'exercice, il suffira de cliquer sur le bouton vert `run`. Cela provoquera l'affichage d'un code QR que l'on cliquera (ou éventuellement scannera avec un smartphone) pour accèder à l'exercice.

Pour revenir au code source, on fermera l'onglet (ou la fenêtre) qui s'est ouverte suite au click sur le code QR. Pour relancer l'application, il suffira de cliquer à nouveau sur le bouton vert (dont le libellé change en fonction du contexte).

*Repl.it* ne nécessite pas la création d'un compte pour être utilisé. Néanmoins, un compte permet d'y stocker ses projets et de les retrouver d'ue session à l'autre. On peut aussi, sans compte, sauvegarder en local l'ensemble d'un projet.

## Les exercices

Les informations données ici viennent en complément de celles indiquées dans le fichier `Student/LISEZMOI.(md|pdf)`.

### Exercice *a*

#### Notions abordées

- Édition d'un fichier source ;
- lancement d'un programme ;
- arrêt d'un programme ;
- booléen,
- création d'une chaîne de caractères ;
- renvoie d'une valeur par une fonction.

#### Solution

```python
def choisirMot():
  return "pied"
```

### Exercice *b*

#### Notions abordées :

- Paramètre de fonction ;
- instruction conditionnelle ;
- taille d'une chaîne de caractères ;
- booléens ;
- opérateur de comparaison ;
- instructions conditionnelles.

#### Solutions

```python
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "bras"
```

Une fois que cette solution aura été trouvée, on pourra proposer de remplacer `if len(suggestion) != 0:` par `if len(suggestion):`, puis par `if suggestion:` en soulignant que c'est une propre à *Python* (et à certains autres langages).

### Exercice *c*

La seconde solution est optionnelle, et pourra être abordée plus tard.

#### Notions abordées

Pour la première version, aucune en particulier ; il s'agit juste d'une amélioration.

Pour la seconde version :

- tuples :
  - création,
  - récupération d'un élément par son index,
  - récupération taille ;
- import d'une fonction particulière d'un module,
- fonction `randint`.

#### Solutions

Première version :

```python
def choisirMot(suggestion,motAuHasard):
  if suggestion:
    return suggestion
  else:
    return motAuHasard
```

seconde version :

```python
from random import randint

…

# Placer les mots de son choix.
DICTIONNAIRE = ("arbre", "maison", "chaise")


def choisirMot(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]
```

### Exercice *d*

#### Notions abordées

- Boucle `for` ;
- opérateur `in` ;
- fonction `range` ;
- index et chaîne de caractères.

#### Solutions

```python
def lettreEstDansMot(lettre,mot):
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True

    return False
```

Une fois cette solution trouvée, on pourra proposer la solution suivante, en soulignant qu'elle est propre à *Python* (et certains autres langages).

```python
def lettreEstDansMot(lettre,mot):
  return lettre in mot
```

### Exercice *e*

#### Notions abordées

- Affectation d'une valeur à une variable ;
- concaténation de chaînes de caractères ;
- éventuellement instruction conditionnelle ternaire.

#### Solutions

```python
def donnerMasque(mot,pioches):
    masque = ""

    for lettre in mot:
        if lettreEstDansMot(lettre,pioches):
            masque = masque + lettre
        else:
            masque = masque + "_"

    return masque
```

Une fois cette solution trouvée, on pourra proposer de remplacer `masque = masque + …` par `masque += …`.

On pourra également, à la place de :

```python
if lettreEstDansMot(lettre,pioches):
    masque += lettre
else:
    masque += "_"
```

proposer :

```python
masque += lettre if lettreEstDansMot(lettre,pioches) else "_"
```

### Exercice *f*

#### Notions abordées

Instructions conditionnelles multiples (équivalent du *case* *switch* d'autres langages).

#### Solutions

```python
def majCorps(nombreErreurs):
  if nombreErreurs == 1:
    partieCorps = P_TETE
  elif nombreErreurs == 2:
    partieCorps = P_TRONC
  elif nombreErreurs == 3:
    partieCorps = P_BRAS_GAUCHE
  elif nombreErreurs == 4:
    partieCorps = P_BRAS_DROIT
  elif nombreErreurs == 5:
    partieCorps = P_PIED_DROIT
  elif nombreErreurs == 6:
    partieCorps = P_PIED_GAUCHE
  elif nombreErreurs == 7:
    partieCorps = P_VISAGE

  dessinerPartieCorps(partieCorps)
```

Autre solution :

```python
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT,
  P_VISAGE
)

def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```
