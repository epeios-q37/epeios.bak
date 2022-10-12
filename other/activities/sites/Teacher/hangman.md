---
menu: Jeu du pendu
title: Jeu du pendu
weight: 2
markup:
  tableOfContents:
    endLevel: 2
    ordered: false
    startLevel: 2

---
<style>
  h2 {
    border-left: 2px solid;
    border-bottom: 2px solid;
    padding: 2px 5px;
  }
  /* Pour Hugo, 'h2' étant utilisé pour le titre du site affiché à gauche. */
  h2.book-brand 
  {
    border: initial;
  }
</style>

# Étapes du jeu du pendu

## Étape *A*

### Particularités

- Édition d'un fichier source ;
- lancement d'un programme ;
- arrêt d'un programme ;
- affectation d'un booléen ;
- création d'une chaîne de caractères ;
- valeur de retour d'une fonction.

### Solution

```python
def choisirMot():
  return "arbre"
```

### Remarques

Veiller à ce que le rôle l'indentation soit bien comprise par les étudiants ; insister que, bien que l'indentation joue un rôle particulier en *Python*, son utilisation est indispensable avec n'importe quel langage pour une bonne compréhension du code.

## Étape *B*

### Particularités

- Paramètre de fonction ;
- instruction conditionnelle ;
- taille d'une chaîne de caractères ;
- booléen :
  - test,
  - transtypage ;
- opérateur de comparaison ;
- instruction conditionnelles.

### Solution

```python
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "arbre"
```

### Remarque

Une fois que la solution ci-dessus trouvée, on pourra proposer de remplacer `if len(suggestion) != 0:` par `if len(suggestion):`, puis par `if suggestion:`, en soulignant que c'est propre à *Python* (et à certains autres langages).

## Étape *C*

### Particularités

Pour la première version, aucune en particulier ; il s'agit juste d'une amélioration.

Pour la seconde version :

- tuples :
  - création,
  - récupération d'un élément par son index,
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

Seconde version :

```python
from random import randint

# …

## Placer les mots de son choix.
DICTIONNAIRE = ("arbre", "maison", "chaise")


def choisirMot(suggestion):
  if suggestion:
    return suggestion
  else:
    return DICTIONNAIRE[randint(0, len(DICTIONNAIRE)-1)]
```

### Remarque

La seconde solution est optionnelle. On pourra y revenir plus tard, une fois les *tuple* abordés. Dans ce cas, ne pas oublier de remettre `from workshop.fr.c import *`…

## Étape *D*

### Particularités

- Boucle `for` ;
- opérateur `in` ;
- fonction `range` ;
- index et chaîne de caractères.

### Solutions

```python
def lettreEstDansMot(lettre,mot):
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True

    return False
```

### Remarque

Une fois cette solution trouvée, on pourra proposer la solution suivante, en soulignant qu'elle est propre à *Python* (et certains autres langages).

```python
def lettreEstDansMot(lettre,mot):
  return lettre in mot
```

## Étape *E*

### Particularités

- Affectation d'une valeur à une variable ;
- concaténation de chaînes de caractères ;
- éventuellement instruction conditionnelle ternaire.

### Solution

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

### Remarque

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


## Étape *F*

### Particularité

Instructions conditionnelles multiples (équivalent du *switch…* *case…* d'autres langages).

### Solution

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

### Remarque

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


## Étape *G*

### Particularités

- Création d'un *tuple* ;
- récupération d'un membre d'un *tuple* par son index ;
- récupération du nombre d'éléments d'un *tuple*.

### Solution

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

### Remarques

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

## Étape *H*

### Particularités

Rien de particulier ; il s'agit juste d'une amélioration.

### Solution

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

### Remarque

On peut également considérer la variante (le second test est imbriqué dans le premier) :

```python
# …

def majCorps(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS):
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

    if nombreErreurs == len(PARTIES_CORPS):
      dessinerPartieCorps(P_VISAGE)
```

Cela permet, dans certains cas, d'éviter un test inutile. Cependant, ce cas se présentant rarement, l'intérêt est limité.

## Étape *I*

### Particularités

- Distinction variables globales/locales ;
- distinction entre une variable contenant une chaîne de caractères, et une variable contenant un entier ;
- problématique [*thread-safety*](https://fr.wikipedia.org/wiki/Thread_safety).

### Solution

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

### Remarques

Les deux premières lignes sont inutiles, la fonction `raz(…)` étant appelée avant toute utilisation d'une des deux variables. Cependant, c'est une bonne pratique pour la clarté du code (permet de repérer rapidement les variables globales).

Veiller à ce que les élèves ne soient pas bloqués par l'absence de la ligne `global bonnesPioches,nombreErreurs` dans les deux fonctions.

Dans un premier temps, le test `if not lettreEstDansMot(pioche,bonnesPioches):` pourra être omis.

Le paramètre `motAuHasard` de la fonction `raz(…)` ne sera peut-être pas utilisé ; cela dépendra de la manière dont la fonction `motAuHasard(…)` aura été codée dans les précédents exercices.

En lançant deux ou plusieurs instances simultanées, du fait que les variables `bonnesPioches` et `nombreErreurs` sont communes à toutes les instances, il sera facile de montrer que les actions effectuées dans une instance influent les autres, notamment :

- des lettres piochées disparaissent ou apparaissent de manière inopinée ;
- le pendu est dessiné d'une manière inconsistante.

À noter que ces problèmes de *thread-safety* ne sont **pas** présents dans les exercices précedents, car les variables `bonnesPioches` et `nombreErreurs` sont directement gérées par des bibliothèques logicielles qui tiennent compte de cette problématique.

## Étape *J*

### Particularités

Programmation orientée objet :

- Classe ;
- fonction membre (méthode) ;
- variable membre ;
- constructeur ;
- utilisation de la *POO* pour gérer le *thread-safety*.

### Solution

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

### Remarques

On peut signaler que `monInstance.maFonction(…)` est équivalent à `maFonction(monInstance,…)`.

## Étape *K*

### Particularités

- Utilisation d'une variable globale comme constante ;
- type booléen ;
- manipulation d'une instance de classe.

### Solution

```python
class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def traiterEtTesterPioche(self,pioche):
    if isLetterInWord(pioche,self.motSecret):
      if not isLetterInWord(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasque(pendu.motSecret,""))

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret( pendu.motSecret )


def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    afficher(donnerMasque(pendu.motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)
```

### Remarques

Néant

## Étape *L*

### Particularités

- Renvoi de plusieurs valeurs par une fonction ;
- construction d'une chaîne de caractères avec le contenu de variables ;
- récupération des valeurs retournées par une fonction.

### Solution

```python
## Remplace 'donnerMasque(…)'
def donnerMasqueEtTesterSiVictoire(mot,pioches):
  masque = ""
  victoire = VRAI

  for lettre in mot:
    if lettreEstDansMot(lettre,pioches):
      masque += lettre
    else:
      masque += "_"
      victoire = FAUX

  return masque,victoire    


## Remplace 'majCorps(…)'
def majCorpsEtTesterSiDefaite(nombreErreurs):
  if nombreErreurs <= len(PARTIES_CORPS)
    dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  return nombreErreurs >= ( P_NOMBRE - 1 )


class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nombreErreurs = 0
    self.enCours = VRAI

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0
    self.enCours = FAUX

  def traiterEtTesterPioche(self,pioche):
    if lettreEstDansMot(pioche,self.motSecret):
      if not lettreEstDansMot(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


def donnerMasqueEtTesterSiVictoire(mot,pioches):
  masque = ""
  victoire = VRAI

  for lettre in mot:
    if lettreEstDansMot(lettre,pioches):
      masque += lettre
    else:
      masque += "_"
      victoire = FAUX

  return masque,victoire


def majCorpsEtTesterSiDefaite(nombreErreurs):
  majCorps(nombreErreurs)

  return nombreErreurs >= ( P_NOMBRE - 1 )


def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    masque,victoire = donnerMasqueEtTesterSiVictoire(pendu.motSecret,pendu.bonnesPioches)
    afficher(masque)
    if victoire and pendu.enCours:
      notifier("Tu as gagné ! Félicitations !")
      pendu.enCours = FAUX
  elif pendu.enCours and majCorpsEtTesterSiDefaite(pendu.nombreErreurs):
    notifier("\nPerdu !\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'."
             .format(pendu.nombreErreurs,len(pendu.bonnesPioches),pendu.motSecret))
    pendu.enCours = FAUX


def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasqueEtTesterSiVictoire(pendu.motSecret,"")[0])

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret(pendu.motSecret)


def AConnexion(pendu,suggestion,motAuHasard):
  return raz(pendu,suggestion,motAuHasard)


def APioche(pendu,pioche):
  traiterPioche(pendu,pioche)


def ARelance(pendu,suggestion,motAuHasard):
  if pendu.enCours:
    notifier("\nErreurs : {} ; bonnes pioches : {}.\n\nLe mot à deviner était : '{}'."
             .format(pendu.nombreErreurs,len(pendu.bonnesPioches),pendu.motSecret))

  raz(pendu,suggestion,motAuHasard)
```

### Remarques

Pour la construction des chaînes de caractères passées à `notifier(…)`, on peut, dans un premier temps, utiliser l'écriture `notifier(… + … + …)`.