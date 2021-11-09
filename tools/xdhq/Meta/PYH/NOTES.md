# Notes concernant la version *Python* (*PYH*)

Concerne le code *Python* <u>et</u> natif.

## Exemple de description en anglais (obsolete):

> Lightweight and dependency-free module to easily add a GUI to Python programs. Requires only basic knowledge of web technologies (no JavaScript knowledge required). CLI and GUI can easily coexist in the same program, so the user can choose the interface s/he wants to use.

## Considérations techniques

### `__main__.py`

En créant, dans le répertoire de l'application, un fichier `__main__.py` contenant juste un `import` du fichier à lancer pour lancer l'application, on peut alors lancer l'application en passant juste le nom du répertoire (avec un `/` final) à la commande `python`.

### *pipenv*

- `python3 -m pipenv shell` : créer un environnement d'exécution python isolé.
- `python3 -m pipenv clean` : désinstalle tout les *package* de l'environnement d'exécution.

## Publication sur *Replit.com*

Pour garder les stats *Spotlight* de *Replit*, il faut toujours garder le *replit* en cours.

Il peut parfois être plus intéressant de modifier/télécharger directement les fichiers plutôt que de suivre la procédure ci-dessous.

### Nouvelle procédure

**Attention** : procéure rédifée de mémoire ; à valider !

- Si existant, effacer `atlas-python-old` ;
- *Forker* `atlas-python` vers `atlas-python-old` ;
- renommer `atlas-python` en `atlas-python-new` ;
- renommer `atlas-python-old` en `atlas-python` ;
- aller dans le shell de `atlas-python-new` ;
- `cd ..` ;
- `git clone https://github.com/epeios-q37/atlas-python` ;
- `rm -rf atlas-python-new/*;mv atlas-python/* atlas-python-new/` ;
- vérifier si `.replit` est identique dans les deux, sinon mettre à jour ;
- renommer `atlas-python` en `atlas-python-old` ;
- renommer `atlas-python-new` en `atlas-python`.

### Ancienne procédure



- Forker le *replit* courant ;
- renommer le *replit* courant en `atlas-python-new` ;
- renommer le *fork* en `atlas-python` ;
- mettre à jour les fichiers (on peut utiliser la console, pour faire des `rm …`, par exemple) ;
- renommer le *fork* en `atlas-python-old` ;
- renommer en `atlas-python` ;
- republier sur *Spotlight*.

Il peut parfois être plus intéressant de modifier les fichiers directement.

On peut importer directement un *repository* *GitHub*, mais ce n'est pas la solution actuellement retenue.

- Créer un nouveau projet *Bash* ;
- déposer le contenu du répertoire `RTW/atlas-python`
  - avec les fichiers cachés, 
  - sans le fichier `.directory` ;
- effacer le fichier `main.sh` ;
- renommer l'actuel *replit* ;
- renommer le nouveau en *atlas-python*.

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

## Ressources

Liste des sites faisant références à la version *Python* du *toolkit* *Atlas*

- <http://github.com/epeios-q37/atlas-python>,
- le wiki sur le site de*Python*: <https://wiki.python.org/moin/GuiProgramming>,
- *Pypi*: <https://pypi.org/project/atlastk/>
- *Curlie* (demande en cours): <https://curlie.org/Computers/Programming/Languages/Python/Modules/GUI/>
