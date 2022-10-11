---
menu: Étape D
title: Jeu du pendu - Étape D
weight: 4
---

# Étape D du jeu du pendu

## Objectifs

Détecter la présence d'une lettre dans un mot.

## Préparation

- Dans la première ligne, remplacer le `c` par un `d` ;
- ajouter `def lettreEstDansMot(lettre,mot):` avant la dernière instruction du fichier.

```python
ETAPE = "d"

m = __import__(f"workshop.fr.{ETAPE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

…

def choisirMot(…):
    …

def lettreEstDansMot(lettre,mot):

go(globals())
```

## Missions

Sachant que :

- `lettre` est la lettre choisie par le joueur ;
- `mot` est le mot à deviner ;

écrire le code de la fonction `lettreEstDansMot` pour que :

- lorsque `lettre` est présent dans `mot`, elle retourne `VRAI` (ou `True`) ;
- lorsque `lettre` n'est **pas** présent dans `mot`, elle retourne `FAUX` (ou `False`).

## Pseudo-code

> Si `lettre` est contenu dans `mot`  
> > Retourner `VRAI` 
>  
> Sinon  
> > Retourner `FAUX`

## Aperçu

Lorsque l'on lance le jeu, voilà ce qui est affiché :

![](../assets/d/Start.png)

- *Pioche* affiche la lettre sélectionnée par l'utilisateur ;
- *Attendu* affiche la valeur que la fonction `lettreEstDansMot` devrait retourner ;
- *Obtenu* affiche ce que la fonction `lettreEstDansMot` retourne effectivement.

Si l'utilisateur clique sur une lettre qui est contenue dans le mot, voici ce qui devrait s'afficher.

![](../assets/d/True.correct.png)

Si la fonction ne retourne pas la bonne valeur, et doit donc être corrigée, alors ceci s'affiche :

![](../assets/d/True.bug.png)

Si l'utilisateur clique sur une lettre qui n'est **pas** contenue dans le mot, voici ce qui devrait s'afficher :

![](../assets/d/False.correct.png)

Si la fonction ne retourne pas la bonne valeur, et doit donc être corrigée, alors ceci s'affiche :

![](../assets/d/False.bug.png)

## Tests

- Cliquer sur une lettre contenue dans le mot :
  
  - *Pioche* doit afficher la lettre cliquée ;
  - *Attendu* doit afficher `vrai` ;
  - *Obtenu* doit afficher `vrai` surligné en vert ;
- cliquer sur une lettre qui n'est **pas** contenue dans le mot :
  
  - *Pioche* doit afficher la lettre cliquée ;
  - *Attendu* doit afficher `faux` ;
  - *Obtenu* doit afficher `faux` surligné en vert.
- refaire les actions ci-dessus avec différentes lettres et différents mots.
  

Si ce qui est affiché dans *Obtenu* est barré et surligné de rouge, alors le code de la fonction `lettreEstDansMot` est incorrect. Noter le mot et le lettre qui posent problème, corriger la fonction, et ressayer le mot et la lettre pour vérifier que le bug a été corrigé.