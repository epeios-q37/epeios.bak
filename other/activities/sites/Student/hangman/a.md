---
menu: Étape A
title: Jeu du pendu - Étape A
weight: 1
---

# Étape A du jeu du pendu

## Objectifs

Se familiariser avec l'édition d'un fichier source, ainsi que le lancement et l'arrêt du jeu.

## Préparation

Ouvrir le fichier `pendu.py`. Il devrait avoir le contenu suivant :

```python
ETAPE = "a"

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

MONTRER_MOT_SECRET = VRAI

def choisirMot():

go(globals())
```

La variable booléenne `DIVULGUER_MOT_SECRET` permet de configurer le jeu pour qu'il affiche le mot à deviner. Sans cette possibilité, il serait compliqué de tester le jeu pour vérifier s'il fonctionne correctement. On donnera à cette variable la valeur `VRAI` (ou `True`) tout au long du développement et de la mise au point du jeu, et `FAUX` (ou `False`) une fois que le jeu sera au point, pour que le mot à deviner ne soit plus affiché.

*Nota* : lancer l'exécution de ce fichier en l'état provoquera l'affichage d'un message d'erreur.

## Missions

Écrire le contenu de la fonction `choisirMot` pour qu'elle retourne toujours la même chaîne de caractères contenant un mot.

## Pseudo-code

> Retourner une chaîne de caractères contenant un mot.

## Aperçu

![](../assets/a.png)

## Tests

- Cliquer sur le bouton `Recommencer` et vérifier que les deux champs affichent le mot retourné par la fonction `choisirMot` ;
- changer le contenu du champ de saisie, cliquer sur le bouton `Recommencer`, et vérifier que les deux champs affichent le même mot que ci-dessus.
