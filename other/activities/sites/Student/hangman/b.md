---
menu: Étape B
title: Jeu du pendu - Étape B
weight: 2
---

# Étape B du jeu du pendu

## Objectifs

Offrir la possibilité de définir le mot à deviner grâce au champ de saisie. C'est-à-dire que, lorsqu'on va cliquer sur le bouton `Recommencer`, c'est le texte contenu dans le champ de saisie qui va être utilisé comme mot à deviner, et donc affiché dans les deux champs.

## Préparation

- Dans la première ligne, remplacer le `a` par un `b` ;
- ajouter le paramètre `suggestion` dans la déclaration de la fonction `choisirMot`.

```python
ETAPE = "b"

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

# …

def choisirMot(suggestion):
    # …

go(globals())
```

## Missions

Sachant que :

- `suggestion` est une chaîne de caractères contenant ce qui a été saisi dans le champ de saisie ;

modifier le code de la fonction `choisirMot` pour que :

- lorsque `suggestion` est vide, elle retourne une chaîne de caractères contenant un mot quelconque, toujours le même (réutiliser le code écrit dans l'étape précédente) ;
- lorsque `suggestion` n'est **pas** vide, elle retourne `suggestion`.

## Pseudo-code

> Si `suggestion` est vide  
> > Retourner un mot quelconque  
> 
> Sinon  
> > Retourner `suggestion`

## Aperçu

Le même que pour l'étape précédente.

## Tests

- Lorsque l'on clique sur `Recommencer` alors que le champ de saisie est vide, c'est toujours le même mot qui doit être affiché dans les deux champs ;
- lorsque l'on clique sur `Recommencer` alors que le champ de saisie n'est **pas** vide, alors c'est le contenu de ce champ qui doit être affiché dans les deux champs.