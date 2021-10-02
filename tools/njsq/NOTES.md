# Notes pour le développeur

## Différences entre `node-gyp` et `node-pre-gyp` (observation partielle)

Les deux ont la même fonction.

`node-pre-gyp` prend en compte une certaine section `binary`  dans `package.json`, section qui est ignorée par `node-gyp`.

Typiquement, appeler `node-gyp configure` avec un `package.json` destiné à `node-pre_gyp`  va génèrer l'erreur `Undefined variable module_name in binding.gyp while trying to load binding.gyp`.

Un `node-gyp build` aprés un `node-pre-gyp configure` fonctionne.

Au final, inutile d'installer `node-gyp` ; installer `node-pre-gyp` suffit.

NOTA: aprés installation respective, `node-gyp` et `node-pre-gyp` se situe dans le répertoire `node_modules/.bin/`.

## Installation

Pour installer la *LTS* de *Node.js* à partir d'un dépôt : https://github.com/nodesource/distributions/

Installer `node-pre-gyp` via `npm`. À faire à partir du répertoire contenant le `package.json` (racine de l'utilitaire).

Toute référence à `node-pre-gyp` dans le reste du document fait référence à `node_modules/.bin/node-pre-gyp`.

## Utilisation

- `DVLGenGYP` : génère le fichier `binding.gyp` — n'est à lancer qu'une seule fois et à chaque changement de version de *Node.js* et/ou modification du fichier `Project.xml` ;
- `node-pre-gyp configure` : génère le contenu du répertoire `build` ;
- `node-pre-gyp build` : génère le fichier `njsq.node`.

Notez que, sous *Windows*, `node-pre-gyp configure` génère un projet *Visual C++* (`.vcxproj`). Par conséquent, on peut utiliser *Visual C++* pour compiler la bibliothèque, mais aussi la déboguer. Par conséquent, il est plus facile de mette au point ce composant sour *Windows*…

Une fois l'*addon* crée (`njsq.node`), un `require("NJSq.js")` va charger cet *addon*. Voir `test.js` ; lancer `node test.js` pour tester.