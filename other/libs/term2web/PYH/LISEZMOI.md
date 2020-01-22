 # Notice de la bibliothèque *term2web* (<https://q37.info/s/dh9gpjft>)

 *term2web* est une bibliothèque *Python* qui redirige les fonctions `print(…)` et `input(…)` sur une page web. De ce fait, on peut mettre en forme le texte affiché par ces fonctions en utilisant des [propriétés CSS](https://fr.wikipedia.org/wiki/Feuilles_de_style_en_cascade).

 Cette bibliothèque peut être utilisée pour familiariser les débutants avec l'environnement de développement d'exercices comme celui se trouvant à l'adresse https://q37.info/s/g9fvwzgp.

 Avec cette bibliothèque, les fonctions `print(…)` et `input(…)` s'utilisent comme d'habitude. Il est ainsi facile de la mettre en œuvre dans les programmes s'exécutant habituellement dans un terminal pour qu'ils s'exécutent dans une page web.

 Dans un second temps, on pourra utiliser les fonctions suivantes pour améliorer la présentation, est initier par la même occasion au CSS.

On a ainsi `set_property(name,value)` qui permet d'appliquer la propriété CSS de nom `name` et de valeur `value`.

Exemple:

```python
set_property("font-style", "italic")
```

Avec `set_properties(properties)`, on applique les propriétés CSS stockées dans `properties`, qui est un dictionnaire dont les clefs sont les noms des propriétés, et les valeurs les valeurs de ces propriétés.

Exemple:

```python
set_properties({
    "text-decoration-line": "line-through",
    "text-decoration-style": "wavy",
    "text-decoration-color": "red"
})
```

Et enfin, `reset_properties()` supprime toutes les propriétés appliquées avec les précédentes fonctions.

Une démonstration en ligne est disponible à l'adresse <https://q37.info/s/kjjcfcp3>. On y trouve `Basic.py`, qui est une exemple d'utilisation de `print(…)` et `input(…)`, sans formatage CSS, et `WithCSS.py`, qui montre comment on applique des propriétés CSS et le résultat.

Pour utiliser cette bibliothèque, on pourra, au choix :

- installer le [paquet *Pypi* correspondant](https://q37.info/s/c7pnhdm7) (`pip install term2web`), puis l'importer (`import * from term2web`) dans votre fichier source ;
- cloner le dépôt *GitHub* <https://github.com/epeios-q37/term2web-python-stub> (`git clone https://github.com/epeios-q37/term2web-python-stub`), et placer votre code source dans le fichier `main.py`, ou placer le lignes contenus dans le fichier `main.py` au début de vos propres fichiers source ;
- faire la même chose, mais en désarchivant le fichier *ZIP* suivant, qui contient le dépôt ci-dessus : <https://q37.info/s/pkcp7qxh> ;
- faire la même chose, en allant à l'adresse suivante, qui donne accès au dépôt ci-dessus dans un environnement de développement en ligne : <https://q37.info/s/7jqrdgnx>.

Contrairement aux autres programmes basés sur le *toolkit* *Atlas*, il n'est pas possible de lancer deux instances simultanées d'un même programme utilisant *term2web*. C'est volontaire, pour ne pas rendre la mise en œuvre de cette bibliothèque plus complexe, puisqu'elle est principalement destinée à être utilisée par des débutants.
 
Si vous avez des suggestions/remarques/commentaires au sujet de cette bibliothèque, n'hésitez pas à en faire part à l'auteur via le formulaire à l'adresse https://q37.info/s/ggq7x4w7.