

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
