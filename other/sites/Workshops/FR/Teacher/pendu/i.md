---
menu: Étape I
title: Jeu du pendu - Étape I
weight: 9
---

# Étape I du jeu du pendu

## Particularités

- Distinction variables globales/locales ;
- distinction entre une variable contenant une chaîne de caractères, et une variable contenant un entier ;
- problématique [*thread-safety*](https://fr.wikipedia.org/wiki/Thread_safety).

## Solution

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

## Remarques

Les deux premières lignes sont inutiles, la fonction `raz(…)` étant appelée avant toute utilisation d'une des deux variables. Cependant, c'est une bonne pratique pour la clarté du code (permet de repérer rapidement les variables globales).

Veiller à ce que les élèves ne soient pas bloqués par l'absence de la ligne `global bonnesPioches,nombreErreurs` dans les deux fonctions.

Dans un premier temps, le test `if not lettreEstDansMot(pioche,bonnesPioches):` pourra être omis.

Le paramètre `motAuHasard` de la fonction `raz(…)` ne sera peut-être pas utilisé ; cela dépendra de la manière dont la fonction `motAuHasard(…)` aura été codée dans les précédents exercices.

En lançant deux ou plusieurs instances simultanées, du fait que les variables `bonnesPioches` et `nombreErreurs` sont communes à toutes les instances, il sera facile de montrer que les actions effectuées dans une instance influent les autres, notamment :

- des lettres piochées disparaissent ou apparaissent de manière inopinée ;
- le pendu est dessiné d'une manière inconsistante.

À noter que ces problèmes de *thread-safety* ne sont **pas** présents dans les exercices précedents, car les variables `bonnesPioches` et `nombreErreurs` sont directement gérées par des bibliothèques logicielles qui tiennent compte de cette problématique.