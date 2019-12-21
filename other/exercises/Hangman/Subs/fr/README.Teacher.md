
# Exercices de programmation du jeu du pendu ; version professeur

Ce jeu d'exercices a été conçu pour s'appuie 

Il n'est pas nécessaire de 


- notions web :
  - navigateur web,
  - champs texte,
  - bouton.
- autres notions :
  - pseudo-code.


---

Remarques à mettre en forme :
- Initiation à la programmation, pas à *Python* ;
- si l'on modifie le code correspondant à un exercice réalisé avant le dernier, on peut se remettre dans le contexte de cet exercice en modifiant le `import`.

---

## Exercice *a*

### Notions abordées

-   Édition d'un fichier source ;
-   lancement d'un programme ;
-   arrêt d'un programme ;
-   création une chaîne de caractère ;
-   renvoie d'une valeur par une fonction.

### Solution

```python
def choisirMot():
  return "pied"
```

## Exercice *b*

### Notions abordées :

-   Paramètre de fonction ;
-   instruction conditionnelle ;
-   taille d'une chaîne de caractère ;
-   booléens ;
-   opérateur de comparaison ;
-   instructions conditionnelles.

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

# Placer les mots de son choix..
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
- indexation d'une chaîne de caractère.

### Solution

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
masque += lettreEstDansMot(lettre,pioches) ? lettre : "_"
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

---


Pre-requis.

- Notions *Python* :
  - chaînes de caractères :
    - tester si la chaîne est vide ou non.
  - variables (constantes) ;
  - tuples :
    - nombre d'éléments d'un tuple ;
    - obtenir un élément à partir de son indice ;
  - commentaires avec '#' ;
  - booléens, avec `True`/`VRAI` et `False`/`Faux`.
- notions web :
  - navigateur web,
  - champs texte,
  - bouton.
- autres notions :
  - pseudo-code.

La démarche derrière cet exercice c'est qu'une difficulté est plus facilement surmonté lorsqu'elle ne fait pas l'objet d'un exercice, mas lorsqu'elle s'inscrit comme une étape pour parvenir à un objectif plus général (à reformuler).

Exercice b :

Montrer l'équivalence `if len(strig) != 0`/`if len(string)`/`if string`

Exercice d :

Premier algo.


```python
for i in range(0,len(mot)):
    if mot[i] == lettre:
        return True

return False
```

puis expliquer l'astuce pour le second algo.

```python
return lettre in mot
```
