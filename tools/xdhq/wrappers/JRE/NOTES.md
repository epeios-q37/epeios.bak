 [Epeios NOTES.md](<file://h:/hg/epeios/notes.md>)

 Script pour *recompiler* l'ensemble des sources *JAVA* (*attention*: le répertoire `classes` doit exister !) :

 `pushd /cygdrive/h/hg/epeios/tools/jreq;rm -rf classes/*;javac -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/wrappers/JRE;rm -rf classes/*;javac -d classes *.java; pushd /cygdrive/h/hg/epeios/tools/jreq;rm -rf classes/*;javac -d classes *.java;cd /cygdrive/h/hg/epeios/tools/xdhq/UnJSq/JRE;rm -rf classes/*;javac -d classes *.java;popd`

 Il y à également des sources dans `tools/xdhq/UnJSq/JRE`, mais elees ne devraient pasd avoir à être modifiés.
 