 # Notes concernant la version *Java* (*JRE*) de *Atlas*/*XDHq*

 ## Compilation

 Script pour *recompiler* l'ensemble des sources *JAVA* (*attention*: le répertoire `classes` **doit** exister !) :

 `pushd /cygdrive/h/hg/epeios/tools/jreq;rm -rf classes/*;javac -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/wrappers/JRE;rm -rf classes/*;javac -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/Atlas/JRE;rm -rf classes/*;javac -d classes *.java;popd`

 Il y a également des sources dans `tools/xdhq/Atlas/JRE`, mais elles ne devraient pas avoir à être modifiés.
 
 ## Lancement

 ### *Cygwin*

 *Atlas* (`info.q37.atlas.Atlas) lance un serveur web via *node.js*, qui est normalement interrompu lors d'un *CTRL-C*, mais cela ne fonctionne pas sous *Cygwin*. Il faut donc lancer le programme sous une session *DOS* classique.

 Si lancé directement (et non pas à l'aide de la commande ci-dessous) sous *Cygwin*, il faudra tuer le serveur *httpd* manuellement.

 Pour contourner ce problème, lancer avec de la manière suivante :
  * pour lancer uniquement : `cmd /c start java <class>`,
  * pour compiler puis lancer en cas de succés : `javac *.java && cmd /c start java <class>`.

 ### `CLASSPATH` pour le développement

 `.;h:/hg/epeios/tools/jreq/classes/;h:/hg/epeios/tools/xdhq/wrappers/JRE/classes/;h:/hg/epeios/tools/xdhq/Atlas/JRE/classes/`