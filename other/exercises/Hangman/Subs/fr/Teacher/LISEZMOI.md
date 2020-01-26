# Exercices de programmation du jeu du pendu - notice enseignant

[![Utiliser en ligne](https://q37.info/s/pcn9vgnj.svg)](https://q37.info/s/jnkcprq4)

## Accès rapide

Cette section permet de retrouver rapidement les différents liens présents dans cette notice.

- Documents enseignant :
  - ce document : <https://q37.info/s/mmdnch9t>,
  - dépôt *GitHub* avec les solutions : <https://q37.info/s/g9fvwzgp>,
  - essayer ces solutions en ligne: <https://q37.info/s/jnkcprq4> ;
- documents élèves :
  - notice : <https://q37.info/s/zmg4m3fx>,
  - dépôt *GitHub* : <https://q37.info/s/93pfmrkq>,
  - ces exercices dans un environnement de développement en ligne : <https://q37.info/s/jzw7knww> ;
- dépôt *GitHub* maître : <https://q37.info/s/7sxtcv7g> ;
- formulaire pour contacter l'auteur : <https://q37.info/s/ggq7x4w7>.



## Présentation

De nos jour, la plupart des jeunes possèdent un smartphone. Il est donc plus facile de les intéresser à la programmation si les exercices se présentent comme les applications qu'ils utilisent sur leurs smartphones.

C'est pourquoi les exercices proposés ici ont une véritable interface graphique, au lieu de l'habituelle et austère interface texte. De plus, avec chaque exercice, un [code QR](https://fr.wikipedia.org/wiki/Code_QR) y donnant accès à partir d'un smartphone est affiché. Ainsi, ils verront qu'ils peuvent utiliser leurs smartphones pour accèder à leurs propres applications, et non pas seulement à des applications comme *Facebook*, *YouTube*, *Twitter*….

En outre, ils peuvent facilement partager cet accès avec leurs proches, directement sur leurs smartphones (ou n'importe quel dispositif équipé d'un navigateur web moderne connecté à internet) respectifs. Ces derniers auront ainsi plus d'occasion de les encourager, et les inciter ainsi à progresser.

Vous trouverez plus de détails sur ce type d'exercices à l'adresse <https://q37.info/s/knf9hdwp>.

Les exercices présentés ici reproduisent les étapes menant à l'élaboration d'un programme, chaque exercice s'appuyant sur le précédent. Ils renforcent l'intérêt des élèves pour la programmation par une utilisation concrète des concepts abordés par les nombreux exercices de programmations que l'on trouve par ailleurs.

Contrairement à la plupart des autres exercices de programmation, chaque exercice ne porte pas sur un concept informatique, mais sur l'élaboration d'une fonctionnalité du programme. L'assimilation des concepts en est ainsi facilitée, car n'étant pas une fin en soi, mais une étape dans la réalisation du programme.

Pour confèrer une dimension ludique à ces exercices, le programme en question est un jeu, le [jeu du pendu](https://fr.wikipedia.org/wiki/Le_Pendu_(jeu)).

Bien que s'appuyant sur le langage *Python*, ces exercices portent sur la programmation en générale, et non pas sur la programmation en *Python*. C'est pour cela que les solutions générales seront données avant les solutions propres à *Python*.

Si vous avez des questions/suggestions/remarques/… concernant ces exercices, vous pouvez contacter l'auteur via le formulaire de contact à l'adresse <https://q37.info/s/ggq7x4w7>, ou utiliser les système de suivi de *GitHub* du dépôt principal à l'adresse <https://github.com/epeios-q37/hangman-exercises>.

## Utilisation

Pour aborder ces exercices, seules des notions de base en développement *Python* sont nécessaires. Ils sont particulièrement indiqués pour faire découvrir ou initier à la programmation.

Le dépôt https://github.com/epeios-q37/hangman-fr est destiné aux élèves. https://github.com/epeios-q37/hangman-fr-teacher reprend le contenu de ce dépôt, en y ajoutant des informations utiles aux enseignants, notamment les solutions des exercices. De ce fait, ce dernier dépôt est réservé aux enseignants.

Pour distribuer ces exercices aux élèves, vous pouvez directement utiliser le dépôt https://github.com/epeios-q37/hangman-fr ou, si vous êtes familiarisés avec *GitHub*, en utiliser les fonctionnalités pour créer votre propre version de ce dépôt. 

Les solutions aux exercices présentées dans cette notice peuvent être vue à l'œuvre en allant à l'adresse <https://repl.it/github/epeios-q37/hangman-fr-teacher>. Les fichiers contenus dans le répertoire `Student` sont ceux mis à disposition des étudiants.

Si vous avez des retours à faire concernant ces exercices, vous pouvez contacter l'auteur à l'adresse <https://q37.info/s/ggq7x4w7>, ou passer par le dépôt maître, d'où sont dérivés les deux dépôts présentés ici, à l'adresse <https://github.com/epeios-q37/hangman-exercises>.

Il existe deux façons pour les élèves de travailler sur ces exercices. Soit en utilisant un ordinateur sur lequel est installé *Python* (utilisation en local), soit dans un navigateur web (utilisation en ligne).

### Utilisation en local

Pour une utilisation en local sur un ordinateur équipé de *Python*, il suffit aux élèves de télécharger et désarchiver le fichier suivant : <https://github.com/epeios-q37/hangman-fr/archive/master.zip>. Alternativement, s'ils possèdent les connaissances adéquates, ils peuvent aussi faire un `git clone https://github.com/epeios-q37/hangman-fr` à partir d'une console.

Les exercices consistent à écrire du code dans le fichier `pendu.py` situé à la racine du dépôt. Pour le lancement, il suffira d'exécuter la commande `python3 pendu.py` dans une console, à partir de la racine du dépôt. S'ouvrira alors un navigateur web leur donnant accès à leur exercice. Le programme sera arrêté avec un *CTRL-C*.

Ce dépôt contient, à la racine, un fichier `LISEZMOI.html` qui leur donne les instructions pour chaque exercice.

### Utilisation en ligne

Pour une utilisation en ligne en utilisant un navigateur web, sans rien avoir à installer, il suffit de suivre le lien suivant : https://repl.it/github/epeios-q37/hangman-fr. Cela ouvre une session dans *Repl.it*, un environnement de développement en ligne.

Le fichier `LISEZMOI.html` leur donnant les instructions pour chaque exercice se situe à l'adresse  https://q37.info/s/zmg4m3fx. Il faut l'ouvrir via cette adresse, car l"accès à ce fichier via *GitHub* ou *Repl.it* n'en affichera que le code source.

Comme ci-dessus, l'exercice consistera à écrire du code dans le fichier `pendu.py`. Pour lancer l'exercice, il suffira de cliquer sur le bouton vert. Cela provoquera l'affichage d'un code QR que l'on cliquera (ou éventuellement scannera avec un smartphone) pour accèder à l'interface de l'exercice.

Pour revenir au code source, on fermera l'onglet (ou la fenêtre) qui s'est ouverte suite au click sur le code QR. Pour relancer l'application, il suffira de cliquer à nouveau sur le bouton vert (dont le libellé change en fonction du contexte).

*Repl.it* ne nécessite pas la création d'un compte pour être utilisé. Néanmoins, ouvrir un compte permet d'y stocker ses projets et de les retrouver d'une session à l'autre. On peut aussi, sans compte, sauvegarder en local l'ensemble d'un projet.

## Les exercices

Les informations données ici viennent en complément de celles indiquées dans le fichier `Student/LISEZMOI.html`, qui est la notice destinée aux étudiants.

**ATTENTION** : ne pas oublier de modifier la ligne `from workshop.fr.… import *` en fonction de l'exercice.

### Exercice *a*

#### Particularités

- Édition d'un fichier source ;
- lancement d'un programme ;
- arrêt d'un programme ;
- affectation d'un booléen ;
- création d'une chaîne de caractères ;
- valeur de retour d'une fonction.

#### Solution

```python
def choisirMot():
  return "arbre"
```

#### Remarques

Veiller à ce que le rôle l'indentation soit bien comprise par les étudiants ; insisté que, bien que l'indentation joue un rôle particulier en *Python*, son utilisation est indispensable avec n'importe quel langage pour une bonne compréhension du code.

### Exercice *b*

#### Particularités

- Paramètre de fonction ;
- instruction conditionnelle ;
- taille d'une chaîne de caractères ;
- booléen :
  - test,
  - transtypage ;
- opérateur de comparaison ;
- instruction conditionnelles.

#### Solution

```python
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "arbre"
```

#### Remarque

Une fois que la solution ci-dessus trouvée, on pourra proposer de remplacer `if len(suggestion) != 0:` par `if len(suggestion):`, puis par `if suggestion:`, en soulignant que c'est propre à *Python* (et à certains autres langages).

### Exercice *c*

#### Particularités

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

Seconde version :

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

#### Remarque

La seconde solution est optionnelle. On pourra y revenir plus tard, une fois les *tuple* abordés. Dans ce cas, ne pas oublier de remettre `from workshop.fr.c import *`…

### Exercice *d*

#### Particularités

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

#### Remarque

Une fois cette solution trouvée, on pourra proposer la solution suivante, en soulignant qu'elle est propre à *Python* (et certains autres langages).

```python
def lettreEstDansMot(lettre,mot):
  return lettre in mot
```

### Exercice *e*

#### Particularités

- Affectation d'une valeur à une variable ;
- concaténation de chaînes de caractères ;
- éventuellement instruction conditionnelle ternaire.

#### Solution

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

#### Remarque

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

#### Particularité

Instructions conditionnelles multiples (équivalent du *switch…* *case…* d'autres langages).

#### Solution

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

#### Remarque

Pour éviter une erreur lorsque cette fonction est appelée avec un nombre d'erreur dépassant le maximum possible (i.e. le nombre de parties de corps disponibles), on testera ce nombre avant de dessiner. La constante `P_NOMBRE` contient le nombre de parties de corps disponibles.

Ainsi, on pourra remplacer :

```python
  dessinerPartieCorps(partieCorps)
```

par :

```python
  if nombreErreurs <= P_NOMBRE:
    dessinerPartieCorps(partieCorps)
```


### Exercice *g*

#### Particularités

- Création d'un *tuple* ;
- récupération d'un membre d'un *tuple* par son index ;
- récupération du nombre d'éléments d'un *tuple*.

#### Solution

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

#### Remarques

Dans un second temps, pour éviter une erreur lors du dépassement du nombre d'erreurs autorisées, on remplacera :

```python
def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```

par :

```python

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS)
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])
```

Noter que, pour cet exercice, et les suivants, on peut stocker le résultat de `len(PARTIES_CORPS)` dans une constante juste aprés la définition de `PARTIES_CORPS`, et utiliser cette constante en lieu et place des appels à `len(PARTIES_CORPS)`.

### Exercice *h*

#### Particularités

Rien de particulier ; il s'agit juste d'une amélioration.

#### Solution

Noter que, par rapport à la solution précédente, `PARTIES_CORPS` ne contient plus la constante `P_VISAGE`.

On pourra faire remarquer que le test sur le nombre d'erreurs autorisés, bien qu'il soit réduit d'un, reste correct, du fait qu'il s'appuie sur le nombre d'éléments contenus dans `PARTIES_CORPS`.

```python
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT
)

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS):
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  if nombreErreurs >= len(PARTIES_CORPS):
    dessinerPartieCorps(P_VISAGE)
```

#### Remarque

On peut également considéré la variante (le second test est imbriqué dans le premier) :

```python
…

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS):
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

    if nombreErreurs == len(PARTIES_CORPS):
      dessinerPartieCorps(P_VISAGE)
```

Cela permet, dans certains cas, d'éviter un test inutile. Cependant, ce cas se présentant rarement, l'intérêt est limité.

### Exercice *i*

#### Particularités

- Distinction variables globales/locales ;
- distinction entre une variable contenant une chaîne de caractères, et une variable contenant un entier ;
- problématique [*thread-safety*](https://fr.wikipedia.org/wiki/Thread_safety).

#### Solution

```python
bonnesPioches = ""
nombreErreurs = 0

def raz(suggestion,motAuHasard):
  global bonnesPioches,nombreErreurs

  bonnesPioches = ""
  nombreErreurs = 0

  motSecret = choisirMot(suggestion,motAuHasard)

  afficher(donnerMasque(motSecret,""))

  return motSecret


def traiterPioche(pioche,motSecret):
  global bonnesPioches,nombreErreurs

  if lettreEstDansMot(pioche,motSecret):
    if not lettreEstDansMot(pioche,bonnesPioches):
      bonnesPioches += pioche
      afficher(donnerMasque(motSecret,bonnesPioches))
  else:
    nombreErreurs += 1
    majCorps(nombreErreurs)
```

#### Remarques

Les deux premières lignes sont inutiles, la fonction `raz(…)` étant appelée avant toute utilisation d'une des deux variables. Cependant, c'est une bonne pratique pour la clarté du code (permet de repérer rapidement les variables globales).

Veiller à ce que les élèves ne soient pas bloqués par l'absence de la ligne `global bonnesPioches,nombreErreurs` dans les deux fonctions.

Dans un premier temps, le test `if not lettreEstDansMot(pioche,bonnesPioches):` pourra être omis.

Le paramètre `motAuHasard` de la fonction `raz(…)` ne sera peut-être pas utilisé ; cela dépendra de la manière dont la fonction `motAuHasard(…)`  aura été codée dans les précédents exercices.

En lançant deux ou plusieurs instances simultanées, du fait que les variables `bonnesPioches` et `nombreErreurs` sont communes à toutes les instances, il sera facile de montrer que les actions effectuées dans une instance influent les autres, notamment :

- des lettres piochées disparaissent ou apparaissent de manière inopinée ;
- le pendu est dessiné d'une manière inconsistante.

À noter que ces problèmes de *thread-safety* ne sont **pas** présents dans les exercices précedents, car les variables `bonnesPioches` et `nombreErreurs` sont directement gérées par des bibliothèques logicielles qui tiennent compte de cette problématique.


### Exercice *j*

#### Particularités


Programmation orientée objet :

- Classe ;
- fonction membre (méthode) ;
- variable membre ;
- constructeur.

#### Solution

```python
class Pendu:
  def raz(self):
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.raz()
    
  def traiterEtTesterPioche(self,pioche,motSecret):
    if isLetterInWord(pioche,motSecret):
      if not isLetterInWord(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz()
  motSecret = choisirMot(suggestion,motAuHasard)
  print(motSecret)
  afficher(donnerMasque(motSecret,""))

  return motSecret
  

def traiterPioche(pendu,pioche,motSecret):
  if pendu.traiterEtTesterPioche(pioche,motSecret):
    afficher(donnerMasque(motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)
```

#### Remarques

On peut signaler que `monInstance.maFonction(…)` est équivalent à `maFonction(monInstance,…)`.

### Exercice *X*

#### Particularités

#### Solution

```python
```

#### Remarques

## Autres exercices

La partie des notices enseignant et élèves concernant ces exercices sont en cours d'élaboration, et devraient être disponibles d'ici une semaine ou deux.
