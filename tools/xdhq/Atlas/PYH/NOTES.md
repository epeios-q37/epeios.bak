# Notes concernant la version *Python* (*PYH*)

Concerne le code *Python* <u>et</u> natif.

## Exemple de description en anglais:

> Lightweight and dependency-free module to easily add a GUI to Python programs. Requires only basic knowledge of web technologies (no JavaScript knowledge required). CLI and GUI can easily coexist in the same program, so the user can choose the interface s/he wants to use.

## Considérations techniques

### `__main__.py`

En créant, dans le répertoire de l'application, un fichier `__main__.py` contenant juste un `import` du fichier à lancer pour lancer l'application, on peut alors lancer l'application en passant juste le nom du répertoire à la commande `python`.

### *Repl.it*

Code à mettre dans le source pour <http://repl.it>.

``` python
# coding: utf-8
"""
IMPORTANT !!!

First of all, fork the application, or it won't work properly!
This can be done:
- by hitting above 'fork' button,
- or, if not available, by modifying this source code.

Once launched, to open the application in a new tab,
click on the QR code at its bottom.

For more details: http://q37.info/s/zbgfjtp9

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)
MIT License
"""

import atlastk as Atlas
import os

os.environ["ATK"]="REPLit" #

def readAsset(path):
  return open(path).read()

```

### *pipenv*

- `python3 -m pipenv shell` : créer un environnement d'exécution python isolé.
- `python3 -m pipenv clean` : désinstalle tout les *package* de l'environnement d'exécution.

## Publication sur *PyPI*

**Effacer, si nécessaire, la variable d'environnement `PYTHONPATH` (`unset PYTHONPATH`).**
**Mettre à jour le numéro de version dans `setup.py` !**

### Packaging

Dans `RTW/atlas-python` :

`python3 setup.py sdist bdist_wheel`

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

#### Publication

Dans `RTW/atlas-python` :

`python3 -m twine upload --repository-url https://test.pypi.org/legacy/ dist/*`

#### Test

- `python -m pipenv clean`
- `python -m pipenv shell`
- `python -m pip install --index-url https://test.pypi.org/simple/ --no-deps atlastk`

### Publication en production

Dans `RTW/atlas-python` :

`python3 -m twine upload dist/*`

## Ressources

Liste des sites faisant références à la version *Python* du *toolkit* *Atlas*

- <http://github.com/epeios-q37/atlas-python>,
- le wiki sur le site de*Python*: <https://wiki.python.org/moin/GuiProgramming>,
- *Pypi: <https://pypi.org/project/atlastk/>
- *Curlie* (demande en cours): <https://curlie.org/Computers/Programming/Languages/Python/Modules/GUI/>
