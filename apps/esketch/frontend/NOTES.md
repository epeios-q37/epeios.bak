# Notes concernant les *frontend*s en général

Concerne les *frontend*s en général, pas uniquement ceux de l'application *eSketch*. Ce fichier n'est pas copié lors de la création d'une nouvelle application.

Les deux seuls *frontend*s qui sont utilisés sont *CLI* et *XDHTML*, stockés dans les répertoires de même nom.

Chacun de ces répertoire contient un fichier `NOTES.md` qui concerne le *frontend* en particulier.



L'entrée `Paramters/Preset/@Handling` peut prendre les valeurs suivantes (ce comportement est géré dans le fichier `global.cpp`) :

- `Show` : affiche *prolog* ;

- `Load` : affiche *login*, avec la zone de sélection du *backend* ;

- `Login` : affiche le *login*, sans la zone de sélection du *backend* ;

- `Run` : affiche la première page de l'application.

En mode multi-utilisateur, seuls les valeurs `Login` et `Run` sont pertinentes. Pour faciliter le développement, dans ce mode, les valeurs `Show` et `Load` sont considérés comme équivalentes à `Login`.
