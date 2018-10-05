# Notes concernant la version *Java* (*JRE*) de *Atlas*/*XDHq*

## <u>Important</u>

Le séparateur de l'argument `-cp`  ou `-classpath` ainsi que du contenu de la variable d'environnement `CLASSPATH`est `;` (point-virgule) **sous Windows** (nécessitant d'entourer le paramètre de `-cp` ou `--classpath` par des quotes sous *Cygwin*).
Pour les autres *OSes*, cela semble être `:` (double-point).

- Pour afficher le fichier et le ligne courante : `System.out.println(Thread.currentThread().getStackTrace()[1]);`
- Code pour le *catch* : `System.out.println(e.getStackTrace());` 

## Compilation

Script pour *recompiler* l'ensemble des sources *JAVA* (*attention*: les répertoires `classes` respectifs **doivent** exister !) :

`ver=1.5;pushd /cygdrive/h/hg/epeios/tools/jreq;rm -rf classes/*;javac -target $ver -source $ver -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/wrappers/JRE;rm -rf classes/*;javac -target $ver -source $ver -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/Atlas/JRE;rm -rf classes/*;javac -source $ver -target $ver -d classes *.java;popd;`

Nom du script : `ATKJCompile`.

## Déploiement

**Après** compilation, commande pour générer le `.jar`:

`pushd /cygdrive/h/temp;rm -rf classes;mkdir classes;cp -R h:/hg/epeios/tools/jreq/classes/* classes/;cp -R h:/hg/epeios/tools/xdhq/wrappers/JRE/classes/* classes;cp -R h:/hg/epeios/tools/xdhq/Atlas/JRE/classes/* classes;popd;jar cvf Atlas.jar -C h:/temp/classes/ .` 

Nom du script : `ATKJPack`.
  
## Lancement

### *Cygwin*

*Atlas* (`info.q37.atlas.Atlas`) lance un serveur web via *node.js*, qui est normalement interrompu lors d'un *CTRL-C*, mais cela ne fonctionne pas sous *Cygwin*. Il faut donc lancer le programme sous une session *DOS* classique.

Si lancé directement (et non pas à l'aide de la commande ci-dessous) sous *Cygwin*, il faudra tuer le serveur *httpd* manuellement.

Pour contourner ce problème, lancer avec de la manière suivante :

- pour lancer uniquement : `cmd /c start java <class>`,
- pour compiler puis lancer en cas de succés : `javac *.java && cmd /c start java <class>`.

## `CLASSPATH` pour le développement

`.;h:/hg/epeios/tools/jreq/classes/;h:/hg/epeios/tools/xdhq/wrappers/JRE/classes/;h:/hg/epeios/tools/xdhq/Atlas/JRE/classes/`