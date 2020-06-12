# Notes concernant la version *Python* (*PYH*)

Concerne le code *Python* <u>et</u> natif.

## Exemple de description en anglais:

> Lightweight and dependency-free module to easily add a GUI to Python programs. Requires only basic knowledge of web technologies (no JavaScript knowledge required). CLI and GUI can easily coexist in the same program, so the user can choose the interface s/he wants to use.

## Considérations techniques

### `__main__.py`

En créant, dans le répertoire de l'application, un fichier `__main__.py` contenant juste un `import` du fichier à lancer pour lancer l'application, on peut alors lancer l'application en passant juste le nom du répertoire à la commande `python`.

### *Repl.it*

Code à mettre dans le source pour <http://repl.it>.

#### Nouvelle version

``` python
# coding: utf-8
"""
To launch the application:
- click on the green button,
- open, in a web browser, the URL displayed
  in the black console (above the '^^…^^' line).

You can also:
- click the 'fork' button, left to the green button,
- uncomment below '#REPLit = True' line (remove
  the leading '#' character),
- click on the green button,
- click on the then displayed QR code.

For more details: http://q37.info/s/zbgfjtp9

Copyright (c) 2019 Claude SIMON (http://q37.info/s/rmnmqd49)
MIT License
"""

#REPLit = True

import atlastk as Atlas
import os

try:
    if REPLit:
        os.environ["ATK"]="REPLit"
except:
    pass

def readAsset(path):
  return open(path).read()
```

#### Anciennes versions

##### Première variante

``` python
# coding: utf-8
"""
IMPORTANT: FORK BEFORE RUNNING !!!

Do not start the program before forking it,
or it won't work properly!

This can be done:
- by hitting above 'fork' button,
- or by modifying this source code.

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

##### Seconde variante

``` python
# coding: utf-8
"""
To launch the application:
- click on the green button (you may have to wait a little until the button is displayed),
- open the URL displayed in the black console (above the '^^^^…' line) in a web browser.

You can also:
- uncomment below 'os.environ["ATK"]="REPLit"' line,
- click on the green button,
- click on the then displayed QR code.

For more details: http://q37.info/s/zbgfjtp9

Copyright (c) 2019 Claude SIMON (http://q37.info/s/rmnmqd49)
MIT License
"""

import atlastk as Atlas
import os

# os.environ["ATK"]="REPLit"

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

**Attention** : vérifier que la version de *wheel* (`pip3 show wheel`) est >= 31.0, sinon mettre à jour (`pip3 install --user --upgrade wheel`), faute de quoi la description en *markdown* n'est pas affichée correctement (mettre à jour *twine* et/ou *setuptools* peut aussi être nécessaire).

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
- *Pypi: <https://pypi.org/project/atlastk/>
- *Curlie* (demande en cours): <https://curlie.org/Computers/Programming/Languages/Python/Modules/GUI/>
