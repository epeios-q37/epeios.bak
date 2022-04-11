# Notes concernant la version *Python* (*PYH*)

Concerne le code *Python* <u>et</u> natif.

> Le répertoire `Examples/AlSweigart` rdt indépendant et contient son propre `NOTES.md` !

## Exemple de description en anglais (obsolete):

> Lightweight and dependency-free module to easily add a GUI to Python programs. Requires only basic knowledge of web technologies (no JavaScript knowledge required). CLI and GUI can easily coexist in the same program, so the user can choose the interface s/he wants to use.

## Considérations techniques

### `__main__.py`

En créant, dans le répertoire de l'application, un fichier `__main__.py` contenant juste un `import` du fichier à lancer pour lancer l'application, on peut alors lancer l'application en passant juste le nom du répertoire (avec un `/` final) à la commande `python`.

### *pipenv*

- `python3 -m pipenv shell` : créer un environnement d'exécution python isolé.
- `python3 -m pipenv clean` : désinstalle tout les *package* de l'environnement d'exécution.

## Publication sur *Replit.com*

**Note concernant la démonstration *MatPlotLib*** : En mode *SpotLight*, le serveur *X* est lancé, bien que non utilisé, cachant la console, et donc l'URL pour visualiser cette démo. Effacer la variable d'environnement `ATK` permet d'afficher l'application dans le navigateur embarqué dans *Rplit*, mais il n'y a alors plus moyen d'afficher la console (même en rechargrant le dépôt *Replit*) et donc de lancer une autre démo…

Pour garder les stats *Spotlight* de *Replit*, il faut toujours garder le *replit* en cours.

Il peut parfois être plus intéressant de modifier/télécharger directement les fichiers plutôt que de suivre la procédure ci-dessous.

Procédures commune à toutes les versions. Remplacer `<flavour>` par l'identifiant de version (`java`, `node`, `python`…).

<u>Ne pas oublier d'éventuellement republier !</u>

### Procédure rapide

**NOTA** : l'équivalent de la procédure ci-dessous peut être effectué en utilisant le *plugin* de *Replit* (bouton *pull*).

- aller dans le shell de `atlas-<flavor>` ;
- `cd ..` ;
- `mkdir new` ;
- `cd new` ;
- `git clone https://github.com/epeios-q37/atlas-<flavor>` ;
- `rm -rf ../atlas-<flavor>/*;mv atlas-<flavor>/* ../atlas-<flavor>/` ;
- vérifier si `.replit` (et éventuels autres fichiers cachés, à l'exclusion du répertoire `.git`) est identique dans les deux, sinon mettre à jour ;
- `cd ..`
- `rm  -rf new`

### Procédure sécurisée

- Si existant, effacer `atlas-<flavor>-old` ;
- *Forker* `atlas-<flavor>` vers `atlas-<flavor>-old` ;
- renommer `atlas-<flavor>` en `atlas-<flavor>-new` ;
- renommer `atlas-<flavor>-old` en `atlas-<flavor>` ;
- aller dans le shell de `atlas-<flavor>-new` ;
- `cd ..` ;
- `git clone https://github.com/epeios-q37/atlas-<flavor>` ;
- `rm -rf atlas-<flavor>-new/*;mv atlas-<flavor>/* atlas-<flavor>-new/` ;
- vérifier si `.replit` (et éventuels autres fichiers cachés, à l'exclusion du répertoire `.git`) est identique dans les deux, sinon mettre à jour ;
- renommer `atlas-<flavor>` en `atlas-<flavor>-old` ;
- renommer `atlas-<flavor>-new` en `atlas-<flavor>`.

## Publication sur *PyPI*

**Effacer, si nécessaire, la variable d'environnement `PYTHONPATH` (`unset PYTHONPATH`).**

- mettre à jour, dans `setup.py` :
  - le numéro de version,
  - éventuellement la description ;
- effacer les affichages de sortie des *notebooks* *Jupyter* ;
- mettre à jour le dépôt *Github*. 

### Packaging

**Attention** : vérifier que la version de *wheel* (`pip3 show wheel`) est >= 31.0, sinon mettre à jour (`pip3 install --user --upgrade wheel`), faute de quoi la description en *markdown* n'est pas affichée correctement (mettre à jour *twine* et/ou *setuptools* peut aussi être nécessaire).

Dans `RTW/atlas-python` :

`python3 setup.py sdist bdist_wheel`

<ins>**IMPORTANT**</ins> — auparavant, effacer :
- les répertoires :
  - `examples`,
  - `tutorials`,
- le fichier `main.py` !!!

Dans le cadre de test, avant publication du *package*, on peut faire un `pip install atlastk-<version>.tar.gz` avec le fichier situé dans le répertoire `dist`.

### Test local

- Dupliquer `RTW/atlas-python`
- `python -m pipenv clean`
- `python -m pipenv shell`
- `unset EPEIOS_SRC PYTHONPATH`
- aller dans le répertoire dupliqué
- faire un test
- supprimer `Atlas.python.zip` et `atlastk`,
- faire un test (échec: `atlastk` manquant)
- `pip install .`
- faire un test.

### Test distant avec l'instance de test

#### Publication sur <u>site de test</u>

Dans `RTW/atlas-python` :

`python3 -m twine upload --repository-url https://test.pypi.org/legacy/ dist/*`

#### Test

- `python -m pipenv clean`
- `python -m pipenv shell`
- `python -m pip install --index-url https://test.pypi.org/simple/ --no-deps atlastk`

### Publication <u>en production</u>

Dans `RTW/atlas-python` :

`python3 -m twine upload dist/*`

## Liste des projets ne s'appuyant pas sur *Pypi*

Ces projets embarquent le *toolkit* *Atlas*, généralement pour en simplifier l'installation, et ne s'appuie pas dur le paquet hébergé sur *Pypi*. La commande `ATKAtlasPack` crée le fichier `Atlas.zip` utilisé par ces projets.

**Attention** : mettre à jour le dépôt *GitHub*, ainsi que l'éventuel dépôt *Replit*.

- https://github.com/epeios-q37/atlas-python-stub ;
- https://github.com/epeios-q37/base-exercise ;
- https://github.com/epeios-q37/basics-exercises ;
- https://github.com/epeios-q37/edutk-python ;
- https://github.com/epeios-q37/hangman-exercises ;
- https://github.com/epeios-q37/hangman-fr ;
- https://github.com/epeios-q37/hangman-fr-teacher ;
- https://github.com/epeios-q37/term2web-python ;
- https://github.com/epeios-q37/term2web-python-stub ;
- https://github.com/epeios-q37/tortoise-python ;
- https://girthub.com/epeios-q37/AlSweigartBigBookPython


## Publications

### Toutes les versions

Liste des sites sur lesquels le *toolkit* *Atlas* est référencé, toutes version confndues :

- *AlternativeTo* :https://alternativeto.net/software/atlas-toolkit/ ;
- *Fresh(cod)e* : https://freshcode.club/search?q=atlas

### Version *Python*

Liste des sites faisant références à la version *Python* du *toolkit* *Atlas*

- <http://github.com/epeios-q37/atlas-python>,
- le wiki sur le site de*Python*: <https://wiki.python.org/moin/GuiProgramming>,
- *Pypi*: <https://pypi.org/project/atlastk/>
- *Curlie* (demande en cours): <https://curlie.org/Computers/Programming/Languages/Python/Modules/GUI/>
