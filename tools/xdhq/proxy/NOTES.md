# Notes concernant le *toolkit* *Atlas*

Concerne actuellement le *toolkit* *Atlas* en tant que bibliothèque de prototypage d'applications web.

## Déploiement des binaires

Il y a deux binaires à déployer sur le serveur distant:

- *xdhwebq* (`tools/xdhwebq/CLI`),
- *xdhqxdh* (`tools/xdhq/proxy`).

Vérifier, avant déploiement, qu'ils comspilent bien sous *Cygwin*.

Mettre à jour les dépôts *GitHub*, puis les cloner sur les serveurs, les compiler et les mettre en place. Laisser tourner la version en cours.

## Le déploiement des bibliothèques utilisateurs

Le déploiement pour *Node.js* doit se faire en premier, car ne nécessite pas l'arrêt du programme.

### *Node.js*

#### *xdhq-node*

Chemin : `tools/xdhq/wrappers/NJS`.

Mettre à jour le dépôt *GitHub* et publier sur *NPM*.

Ignorer les messages concernant l'absence de `runkit.js` et `test.js` lors de la mise à jour.
Contrôler le changement de version dans `package.json`.

#### *atlas-node*

Chemin : `tools/xdhq/Atlas/NJS`.

- Mettre le numéro de version à jour dans `Project.xml`,
- copier le cœur de `Hello.js` dans `runkit.js` en veillant:
  - à mettre à jour la version de *atlastk* dans le `require(...)`,
  - à la présence du texte sur le *timeout* de *RunKit* à la fin du programme,

Avant publication sur *NPM*, vérifier :

- dans `package.json` :
  - que le numéro de version est le bon,
  - que le numéro de version de `xdhqnjs` est également le bon,
- dans `runkit.js`, que le numéro de version de `atlastk` est le bon.

Á ce moment-là, il convient probablement de mettre à jour les binaires, voire de basculer sur la version de maintenance de `xdh.php`.

### *Java*

Après avoir lancées en local les différentes applications pour voir si elles fonctionnent correctement :

- `ATKJCompile`, (normalement déjà fait avant de lancer les tests),
- `ÀTKJPack`.,
- transférer `~/Atlas.jar` dans le répertoire `assets` sur *q37.info*.

### *PHP*

Après avoir lancées en local les différentes applications pour voir si elles fonctionnent correctement :

- `ÀTKZPack`.,
- transférer `~/Atlas.phar` dans le répertoire `assets` sur *q37.info*.