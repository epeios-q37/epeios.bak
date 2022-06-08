# Notes concernant le *toolkit* *Atlas*

Concerne actuellement le *toolkit* *Atlas* en tant que bibliothèque de prototypage d'applications web.

## Déploiement des binaires

### Cas général

Il y a deux binaires à déployer sur le serveur distant:

- [*xdhwebq*](http://github.com/epeios-q37/xdhwebq-cli) (`tools/xdhwebq/CLI`),
- [*xdhqxdh*](http://github.com/epeios-q37/xdhqxdh) (`tools/xdhq/proxy`).

Vérifier, avant déploiement, qu'ils compilent bien sous *Cygwin*.

Mettre à jour les dépôts *GitHub*, puis les cloner sur les serveurs, les compiler et les mettre en place. Laisser tourner la version en cours.

### VPS *bsddev* (*berny.mcs.com.ve*)

On peut tester les binaires lancés sur *bsddev* en utilisant le serveur web local. Le fichier `xdhwebq.js` et la configuration *nginx* ont été modifiée en conséquence. Cependant, il faut :
- sur *bsddev*, modifier `xdhqxhd.xcfg` pour que l'URL pointe sur `…xdh_bsddev.php…` au lieu de `…xdh.php…` ;
- lancer localement `redir --lport 53700 --caddr=berny.mcs.com.ve --cport 53700` ;

## Le déploiement des bibliothèques utilisateurs

Le déploiement pour *Node.js* doit se faire en premier, car ne nécessite pas l'arrêt de l'application.

### *Node.js*

#### [*xdhq-node*](http://github.com/epeios-q37/xdhq-node)

Chemin : `tools/xdhq/wrappers/NJS`.

Mettre à jour le dépôt *GitHub* et publier sur *NPM*.

Ignorer les messages concernant l'absence de `runkit.js` et `test.js` lors de la mise à jour.
Contrôler le changement de version dans `package.json`.

#### [*atlas-node*](http://github.com/epeios-q37/atlas-node)

Chemin : `tools/xdhq/Atlas/NJS`.

- Mettre le numéro de version à jour dans `Project.xml`,
- copier le cœur de `Hello.js` dans `runkit.js` en, veillant à la présence du texte sur le *timeout* de *RunKit* à la fin du programme,

Avant publication sur *NPM*, vérifier dans `package.json` :

- que le numéro de version est le bon,
- que le numéro de version de `xdhqnjs` est également le bon.

Á ce moment-là, il convient probablement de mettre à jour les binaires, voire de basculer sur la version de maintenance de `xdh.php`.

### *Java*

Après avoir lancées en local les différentes applications pour voir si elles fonctionnent correctement :

- `ATKJCompile`, (normalement déjà fait avant de lancer les tests),
- `ATKJPack`,
- transférer dans le répertoire `assets` sur *q37.info* :
  - `~/Atlas.jar`,
  - si modifié, `Hello.java`,
- mettre à jour les dépôts *GitHub* :
  - [*xdhq-java*](http://github.com/epeios-q37/xdhq-java) (`/tools/xdhq/wrappers/JRE`),
  - [*atlas-java*](http://github.com/epeios-q37/atlas-java) (`tools/xdhq/Atlas/JRE`)

### *PHP*

Après avoir lancées en local les différentes applications pour voir si elles fonctionnent correctement :

- `ATKZPack`,
- transférer dans le répertoire `assets` sur *q37.info* :
  - `~/Atlas.phar`,
  - si modifié, `Hello.php`,
- mettre à jour les dépôts *GitHub* :
  - [*xdhq-php*](http://github.com/epeios-q37/xdhq-php) (`/tools/xdhq/wrappers/PHP`),
  - [*atlas-php*](http://github.com/epeios-q37/atlas-php) (`tools/xdhq/Atlas/PHP`).

### Contrôle

Dans une console *DOS*, vérifier si les instructions données dans les *README* des dépôts suivant fonctionnent :

- <http://github.com/epeios-q37/atlas-java>,
- <http://github.com/epeios-q37/atlas-node>,
- <http://github.com/epeios-q37/atlas-php>.

## Miscellaneous examples

Mettre à jour les dépôts suivants, puis contrôler :

- [*atlas-java-demo*](http://github.com/epeios-q37/atlas-java-demo),
- [*atlas-node-demo*](http://github.com/epeios-q37/atlas-node-demo),
- [*atlas-php-demo*](http://github.com/epeios-q37/atlas-php-demo).

## Chat room

## *TodoMVC*

Mettre à jour les dépôts suivants, puis contrôler :

- [*todomvc-java*](http://github.com/epeios-q37/todomvc-java),
- [*todomvc-node*](http://github.com/epeios-q37/todomvc-node),
- [*todomvc-php*](http://github.com/epeios-q37/todomvc-php).

## Chat room

Mettre à jour les dépôts suivants, puis contrôler :

- [*chatroom-java*](http://github.com/epeios-q37/chatroom-java),
- [*chatroom-node*](http://github.com/epeios-q37/chatroom-node),
- [*chatroom-php*](http://github.com/epeios-q37/chatroom-php).
