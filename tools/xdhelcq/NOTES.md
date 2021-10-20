# Notes concernant *XDHELCq*

## Lancement

### Directement à partir de la ligne de commande

Le chemin vers *Electron* est `node_modules/.bin/electron`.

Le module doit être lancé comme valeur de l'option `--module|-m`.

Le module doit être donné <u>avec</u> son chemin, ou sinon les fichiers de locale et de configuration ne seront pas trouvé.

Le second (troisième avec l'option `-m|--module`) arguments doit être `index.js`. Dans le cas contraire, c'est `XDHElcQ.js` qui sera utilisé, étant le fichier déclaré dans la section `main` du fichier `package.json` afin que le `require('xdehelcq')` fonctionne correctement.

Comme dernier argument, il faut fournir le chemin du répertoire contenant tout les fichiers lus directement par *Electron*.

Exemple du lancement du *frontecnt*  *Atlas* pour *TodoMVC* : `node_modules/.bin/electron index.js -m=h:/bin/xdhqxdh TodoMVC`.


### En utilisant le *debugger* *Code::Blocks*

- Dans *Host application:* : `/media/csimon/H/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron` ;
- dans *Program arguments:* : `/home/csimon/epeios/tools/xdhelcq/index.js -m=/home/csimon/bin/libesketchxdh.so /home/csimon/epeios/apps/esketch/frontend/XDHTML/ `.

## Installation

`npm install --save-dev electron-rebuild` : installe *electron-rebuild*. mais également *electron* dansle répertoire `node-modules` ; `electron` et `electron-rebuild` sont stockés dans `./node-modules/.bin/`.


Install, **in this directory, without the `-g` option**, the *NPM* packages *electron* and _electron-rebuild_ (`npm install electron electron-rebuild`).

## Compilation

Sous *Linux* (uniquement ?), `electron-reuild` va lancer la compilation. Une fois le répertoire `build` crée, on peut également utiliser `make [-j…] -C build`.

Launch `DVLGenGYP`. `electron` (and also `electron-rebuild`) will be in `nodes_modules/.bin/`. Although `DVLGenGYP` builds the wrapper, rebuild it with the IDE from *Visual Studio* in order for the binary to be deployed correctly.

`node_module/.bin/electron-rebuild` is an alternative way to build the addon, but `DVLGenGYP` ìs required to create the proper `.sln`, and also to rebuilding with *Visual Studio*.

The `build` can be safely deleted ; it will be rebuild (no pun intended). This may solve some problems.

Resources:

* <https://electron.atom.io/docs/tutorial/using-native-node-modules/>
* <https://github.com/juliangruber/require-rebuild>

## Caveats:

**If you have the message `…/xdhelcq.node is not a valid Win32 application.` is displayed the arguments, this may only mean that the arguments are incorrect !!!**

If the build fails, retry after deleting `.electron`, `.electron-gyp` and `.node-gyp` in `C:/Users/csimon/`.
