# Exercices de programmation du jeu du pendu ; documentation professeur

## Introduction

De nos jour, la plupart des jeunes possèdent un smartphone. Il sera donc plus facile de les intéresser à la programmation avec des exercices qui s'utilisent comme les applications de leurs smartphones.

C'est pourquoi les exercices présentés ici ont une véritable interface graphique, au lieu de l'habituelle interface texte. En outre, chaque exercice, à son lancement, se voit automatiquement doté d'un [code QR](https://fr.wikipedia.org/wiki/Code_QR) qui permet d'y accèder à partir d'un smartphone. Cela permet de mettre en évidence, par l'exemple, le fait que l'ont peut utiliser son smartphone pour utiliser ses propres applications, et non pas seulement des applications comme facebook, instagram, twitter….

En outre, le fait de pouvoir facilement montrer leur travail à leurs proches en leur donnant la possibilité d'y accèder avec leurs smartphones (ou n'importe quel dispositif équipé d'un navigateur web moderne connecté à internet) respectifs les motiveront à explorer les possibi

De plus, comme ils peuvent facilement en leur montrant qu'ils peuvent facielement on peut les encourager à montrer leur travail à leurs proches, en leur donnant accès à ces exercices sur leurs smartphones .

Vous trouverez plus de détails sur les caractéristiques de ce nouveau type d'exercices à cette adresse : https://q37.info/s/knf9hdwp.

Ces exercices aboutissent à la programmation du jeu de pendu avec une interface graphique, sans avoir à manipuler directement cette interface. Chaque exercice s'appuie sur le précédent et l'ensemble reproduit les différentes étapes habituellement suivies pour la réalisation d'un tel jeu.

Ces exercices portent la programmation en générale, et non pas sur l'utilisation de *Python*. C'est pour cela que les solutions générales sont donnés avant les solutions propres à *Python*, et ils conviendra d'en expliquer la différence.

## Utilisation

Le dépôt https://github.com/epeios-q37/hangman-fr est destinés aux élèves. https://github.com/epeios-q37/hangman-fr-teacher reprend le contenu de ce dépôt, en y ajoutant des informations utiles aux professeurs, notamment les solutions des exercices. De ce fait, ce dernier dépôt est réservé aux professeurs.

Pour distribuer ces exercices aux élèves, vous pouvez directement utiliser le dépôt https://github.com/epeios-q37/hangman-fr. Si vous être familiarisé avec *GitHub*, vous pouvez utiliser les outils mis à disposition pour créer votre propre dépôt. Dans ce cas, ils vous suffit de remplacer `epeios-q37` par votre nom d'utilisateur sur *GitHub*, et `hangman-fr` par le nom que vous avez donné à votre dépôt.

Ls exercices ne sont pas conçus por être enchaînés directement, mais au fur et à mesure que les différentes notions informatiques auront été abordées par ailleurs. Vous pouvez tester ces exercices en allant à cette adresse : https://repl.it/github/epeios-q37/hangman-fr-teacher. Les fichiers contenus dans le répertoire `Student` `LISEZMOI.Student.pdf` sint ceux mis à disposition des étudiants.

### Utilisation en local

Pour une utilisation en local, sur un ordinateur équipé de *Python*, il suffit aux élèves de télécharger et désarchiver le fichier suivant : https://github.com/epeios-q37/hangman-fr/…. Alternativement, s'ils les connaissances adéquates, ils peuvent aussi faire un `git clone https://github.com/epeios-q37/hangman-fr`.

Les exercices consistent à écrire du code dans le fichier `pendu.py` situé à la racine du dépôt. Pour le lancement, il suffira d'exécuter la commande `python3 pendu.py` dans une console, à partir de la racine du dépôt. S'ouvrira alors un navigateur web leur donnant accès à leur exercice. Le programme sera arrêté avec un *CTRL-C*.

Ce dépôt contient, à la racine, un fichier PDF `LISEMOI.pdf` qui leur donnent les instructions pour chaque exercice.

### Utilisation en ligne

Pour une utilisation en ligne, en utilisant un navigateur web, sans rien avoir à installer, il suffit de suivre le lien suivant : https://repl.it/github/epeios-q37/hangman-fr.

Comme ci-dessus, l'exercice consistera à écrire du code dans le fichier `pendu.py`. Pour lancer l'exercice, il suffira de clique sur le bouton vert `run`. Cela provoquera l'affichage d'un code QR que l'on cliquera (ou éventuellement scannera avec un smartphone) pour accèder à l'exercice.

Pour revenir au code source, on fermera l'onglet (ou la fenêtre) qui s'est ouverte suite au clock sur le code QR. Pour relancer l'application, il suffira de cliquer sur le bouton vert (dont le libellé change en fonction du contexte).

## Exercice *a*

### Notions abordées

- Édition d'un fichier source ;
- lancement d'un programme ;
- arrêt d'un programme ;
- création une chaîne de caractères ;
- renvoie d'une valeur par une fonction.

### Solution

```python
def choisirMot():
  return "pied"
```

## Exercice *b*

### Notions abordées :

- Paramètre de fonction ;
- instruction conditionnelle ;
- taille d'une chaîne de caractères ;
- booléens ;
- opérateur de comparaison ;
- instructions conditionnelles.

### Solutions

```python
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "bras"
```

Une fois que cette solution aura été trouvée, on pourra proposer de remplacer `if len(suggestion) != 0:` par `if len(suggestion):`, puis par `if suggestion:` en soulignant que c'est une propre à *Python* (et à certains autres langages).

## Exercice *c*

La seconde solution est optionnelle, et pourra être abordée plus tard.

### Notions abordées

Pour la première version, aucune en particulier ; il s'agit juste d'une amélioration.

Pour la seconde version :

- tuples :
  - création,
  - indexation,
  - récupération taille ;
- import d'une fonction particulière d'un module,
- fonction `randint`.

### Solutions

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

## Exercice *d*

### Notions abordées

- Boucle `for` ;
- opérateur `in` ;
- fonction `range` ;
- index et chaîne de caractères.

### Solutions

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

## Exercice *e*

### Notions abordées

- Affectation d'une valeur à une variable ;
- concaténation de chaînes de caractères ;
- éventuellement instruction conditionnelle ternaire.

### Solutions

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

## Exercice *f*

### Notions abordées

Instructions conditionnelles multiples (équivalent du *case* *switch* d'autres langages).

### Solutions

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
