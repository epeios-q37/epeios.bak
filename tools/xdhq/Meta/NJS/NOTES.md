# Developers/maintainers notes about the *Atlas* toolkit binding for *Node.js*


La publication d'une nouvelle version du *toolkit* *Atlas* pour *Node.js* consiste en deux procédures :
- publication du paquet *NPM*,
- publication sur *GitHub*.

Ces deux procédures vont êtres détaillées ci-dessus.

**La première procédure (publication sur *NPM*) est indispensable à la seconde !**

La version du wrapper binaire (*C++*) du *toolkit* *Atlas* n'est plus actuellement maintenue et donc publiée.

## Publication sur *NPM*

### Préparation

- Mettre à jour le numéro de version dans le fichier `Atlas/package.json` ;
- `ATKNJSRTW` ;

### Test

Dans `RTW/atlas-node`
- `npm pack atlastk/` ;
- remplacer l'entrée `Dependencies/atlastk` dans `package.json` par `atlastk-…-.tgz` ;
- `unset Q37_EPEIOS` ;
- `node Hello/Hello.js` doit échouer ! ;
- `npm install` ;
- `node Hello/Hello.js` doit fonctionner.

### Publication
- `npm publish atlastk/`.


## Publication sur *GitHub*

### Test

- `ATKNJSRTW` ;
- créer un *REPL* bash sur *REPLit* ;
- copier tout le contenu de `RTW/atlas-node`(ne pas oublier `.replit`) dans le *REPL* ;
- tester.

### Publication

Dans `RTW/atlas-node` :
- `ATKNJSRTW` ;
- `RTWUpdate` ;
- `git gui` et contrôler,
- `RTWCommit`.

---

**Above this line are the old publication procedures.**

---

## New packaging

- *xdhq-node* (paquet *NPM*: *xdhq*):
  - le numéro devrait se mettre à jour tout seul, sinon définir temporairement sa valeur dans le fichier projet du répertoire ci-dessous ; **Attention** :
    - ~~`mm` (si `yyyy` identique) **doit** être différent de celui en cours, sinon *RunKit* ne prendra pas le  *package* en compte,~~ plus d'actualité, vu que c'est *Repl.it* qui est utilisé pour les *live demonstrations*,
    - si modifié manuellement, le numéro doit être au format `yyyymmdd` dans le fichier projet,
  - dans `epeios/tools/xdhq/wrappers/NJS` : `DVLRTW[_N(C|G)]` ;
  - dans `RTW/xdhq-node` : `npm publish` ;
- *atlas-npm*
  - mettre à jour le numéro de version dans le fichier projet du répertoire ci-dessous,
  - dans `epeios/tools/xdhq/Atlas/NJS` : `DVLRTW_N(C|G)` (pas de `DVLRTW` à ce stade !),
  - dans `RTW/atlas-npm` : `npm pack`, et faire une simulation du *atlas-node* ci-dessous en modifiant le `package.json` pour le faire pointer sur le *package* génèré,
  - dans `RTW/atlas-npm` : `npm publish`; ~~**ATTENTION**: avant publication, vérifier dans *RunKit* que la dernière version du module *xdhq* est bien la dernière !!!~~ plus d'actualité, vu qu'on utilise *Repl.it*" pour les *live demonstrations*,
  - dans `epeios/tools/xdhq/Atlas/NJS` : `DVLRTW`
- *atlas-node*:
  - **ATTENTION**: mettre à jour le `README.md`, notamment le numéro de version (le badge, deux modifications à faire) !
  - `ATKNJSRTW`,
  - dans `RTW/atlas-node` : `ATKRTWUpdate`,
  - vérifier le contenu (faire également un *git diff*)
  - `ATKRTWCommit`
  - clôner et tester.

### *Repl.it*

Code à mettre dans le source pour <http://repl.it>.

**Pour le commentaire en tête de fichier, voir celui de *Python*** (ne pas oublier de modifier la ligne à décommenter!

#### Nouvelle version

``` javascript
//var REPLit = true;

var REPLit = REPLit || false;

if ( REPLit )
    process.env["ATK"]="REPLit";

const atlas = require('atlastk');
```

#### Ancienne version

``` javascript
// process.env["ATK"]="REPLit";

const atlas = require('atlastk');

function readAsset(fileName) {
  return Buffer.from(fs.readFileSync(fileName)).toString();
}

```

## The *README* files

- `README.md` is for the `atlas-npm` repository (the one used as source for the *NPM* package),
- `README-node.md` is renamed and used for the `atlas-node` package.

----

## New packages

Currently, the new packaging is being developed for the *Node.js* binding. It will later be developed for the other bindings.

As the core of the *Atlas* toolkit is written *C/C++*, and the package manager for the different language (*NPM* for *Node.js*, *Composer* for *PHP*...) aren't designed to deploy native code, the installation process is somewhat cumbersome.

To allow those who are interested to fiddle with the *Atlas* toolkit, all the native component are on a server, and only components written in the language of the binding, an d hence easily installable with the package manager for this language, is locally installed.

This default installation can also be used for prototyping, for demonstrating a application to remote client, to ameliorate an application. The core of the application is executed locally, so no need to upload the source code to a remote server.

This default installation is not fitted for production, for which a full installation will be preferred. The desktop version also need the full installation.

To the currently available value for the command-line argument (`web`, `desktop`, `none` and corresponding shortcuts `w`, `d`, `n` and `wd` for both) can be prefixed with `P` to launch explicitly the *PROD* (production mode, which needs the full installation) or with `D` for the *DEMO* (demonstration) mode.

The developer's default mode id *PROD*, for all the others, it's *DEMO*.

## Current/old packages

Dependencies :

* *njsq*, **à packager en premier !!!**:
  * *wrapper* *Node.js*,
  * `njsq/`
  * <http://github.com/epeios-q37/njsq/>,
  * [![NPM](https://nodei.co/npm/njsq.png)](https://nodei.co/npm/njsq/).

* *xdhqxdh*:
  * *proxy*,
  * `xdhq/proxy/`,
  * <http://github.com/epeios-q37/xdhq/>,
  * [![NPM](https://nodei.co/npm/xdhqxdh.png)](https://nodei.co/npm/xdhqxdh/).

* *xdhqnjs*:
  * *wrapper* *XDHTML*,
  * `xdhq/wrapper/NJS/`
  * <http://github.com/epeios-q37/xdhq-node/>,
  * [![NPM](https://nodei.co/npm/xdhqnjs.png)](https://nodei.co/npm/xdhqnjs/).

* *xdhwebqnjs*:
  * *frontend* *web* *XDHTML*,
  * `xdhwebq/NJS`,
  * <https://github.com/epeios-q37/xdhwebq-node>
  * [![NPM](https://nodei.co/npm/xdhwebqnjs.png)](https://nodei.co/npm/xdhwebqnjs/).

* *xdhelcq*:
  * *frontend* *desktop* *XDHTML*,
  * `xdhwelcq`,
  * <https://github.com/epeios-q37/xdhelcq>
  * [![NPM](https://nodei.co/npm/xdhelcq.png)](https://nodei.co/npm/xdhelcq/).

* *atlasnjs*:
  * *wrapper* *XDHTML* + serveur *httpd* + *Electron*,
  * `xdhq/Atlas/NJS/`
  * <http://github.com/epeios-q37/atlas-node/>,
  * [![NPM](https://nodei.co/npm/atlastk.png)](https://nodei.co/npm/atlastk/).

*NOTA*:

* Following the installation of all *Atlas* toolkit bindings through *NPM*, the *xdhq* package was renamed to *xdhqnjs*,
* Because `require('atlas-node')` was to clumsy, the `atlas-node` package was renamed to `atlastk`,
* `xdhwebqnjs`was first `xdhwebq`.