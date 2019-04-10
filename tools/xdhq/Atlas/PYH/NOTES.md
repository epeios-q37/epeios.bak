# Notes concernant la version *Python* (*PYH*)

Concerne le code *Python* <u>et</u> natif.

## `__main__.py`

En créant, dans le répertoire de l'application, un fichier `__main__.py` contenant juste un `import` du fichier à lancer pour lancer l'application, on peut alors lancer l'application en passant juste le nom du répertoire à la commande `python`.

## *pipenv*

- `python3 -m pipenv shell` : créer un environnement d'exécution python isolé.
- `python3 -m pipenv clean` : désinstalle tout les *package* de l'environnement d'exécution.

## Publication sur *PyPI*

**Mettre à jour le numéro de version dans `setup.py` !**

### Packaging

Dans `RTW/atlas-python` :

`python3 setup.py sdist bdist_wheel`

### Test local

Dans une session *DOS* :

- `python -m pipenv clean`
- `python -m pipenv shell`
- `pip install h:\RTW\`

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

